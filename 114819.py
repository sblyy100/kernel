
from iotop.netlink import *       # pylint: disable=W0614
from iotop.genetlink import *     # pylint: disable=W0614

CTRL_ATTR_MCAST_GROUPS = 7

CTRL_ATTR_MCAST_GRP_UNSPEC = 0
CTRL_ATTR_MCAST_GRP_NAME   = 1
CTRL_ATTR_MCAST_GRP_ID     = 2


class MyController(Controller):
    def __init__(self, conn):
        self.conn = conn
        self.grp_ids = {}
        Controller.__init__(self, conn)

    def print_mcastgroup(self, mcastgroups):
        print "mcastgroups ", mcastgroups
        for key in mcastgroups.iterkeys():
            value = mcastgroups[key]
            innerattr = value.nested()
            for innerkey in innerattr.iterkeys():
                innervalue = innerattr[innerkey]

    def print_message(self, m):
        try:
            mcastgroups =  m.attrs[CTRL_ATTR_MCAST_GROUPS].nested()
            self.print_mcastgroup(mcastgroups)
        except Exception:
            return

    def get_group_id(self, grp_name):
        return self.grp_ids[grp_name]

    def get_family_id(self, family):
        a = NulStrAttr(CTRL_ATTR_FAMILY_NAME, family)
        m = GeNlMessage(GENL_ID_CTRL, CTRL_CMD_GETFAMILY,
                        flags=NLM_F_REQUEST, attrs=[a])
        m.send(self.conn)
        m = GeNlMessage.recv(self.conn)

        # while we have it, remember group names and ids
        mcastgroups =  m.attrs[CTRL_ATTR_MCAST_GROUPS].nested()
        for key in mcastgroups.iterkeys():
            value = mcastgroups[key]
            innerattr = value.nested()
            # remove trailing '\0' from grp name
            grp_name = innerattr[CTRL_ATTR_MCAST_GRP_NAME].str().rstrip('\0')
            grp_id   = innerattr[CTRL_ATTR_MCAST_GRP_ID].u32()
            self.grp_ids[grp_name] = grp_id
        return m.attrs[CTRL_ATTR_FAMILY_ID].u16()

def _genl_hdr_parse(data):
    return GenlHdr(*struct.unpack("BBxx", data))

RTM_NEWNEIGH = 28
RTM_DELNEIGH = 29
RTM_GETNEIGH = 30

NDA_UNSPEC    = 0
NDA_DST       = 1
NDA_LLADDR    = 2
NDA_CACHEINFO = 3
NDA_PROBES    = 4

def parse_payload2(payload):
    ip = "0.0.0.0"
    macaddr = "000000"

    ndmsg = payload[:12]
    family, ifindex, state, flag, type = struct.unpack('I I H B B', ndmsg)

    stuff = payload[12:]

    attr = parse_attributes(payload[12:])
    for key in attr.iterkeys():
        if key == 1: # IP addr
            ipb = struct.unpack('BBBB', attr[key].data)
            ip = "%s.%s.%s.%s" %(ipb[0], ipb[1], ipb[2], ipb[3])
        elif key == 2: # MAC addr
            macb = struct.unpack('BBBBBB', attr[key].data)
            macaddr = "%0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x" %(macb[0], macb[1], 
                                                              macb[2], macb[3], 
                                                              macb[4], macb[5])

    return ifindex, family, ip, macaddr
        


def print_msg(msg):
    if msg.type == RTM_GETNEIGH:
        ifindex, family, ip, macaddr = parse_payload2(msg.payload)
        print "index: %s" %ifindex
        print "family: %s" %family
        print "ip: %s" %ip
        print "mac: %s" %macaddr


if __name__ == '__main__':

    RTNLGRP_NEIGH = 3
    group_id = RTNLGRP_NEIGH
    print "group_id  ", group_id
    
# open socket to receive only the message we are interested in
    mcast_grp = 1 << (group_id - 1)
    print "Subscribing to %s" % mcast_grp
    mcastconnection = Connection(NETLINK_ROUTE, mcast_grp)
    mcastcontroller = MyController(mcastconnection)

    
# get reponse 
    while 1:
        msg = mcastconnection.recv()
        mcastcontroller.print_message(msg)
        print_msg(msg)
