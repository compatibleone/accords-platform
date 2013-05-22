###############################################################################
# copyright 2012, Marouen Mechtri (Marouen.Mechtri@it-sudparis.eu)            #
# Institut Mines-Telecom - TELECOM & Management SudParis                      #
# Licensed under the Apache License, Version 2.0 (the "License");             #
# you may not use this file except in compliance with the License.            #
# You may obtain a copy of the License at                                     #
#                                                                             #
#       http://www.apache.org/licenses/LICENSE-2.0                            #
#                                                                             #
# Unless required by applicable law or agreed to in writing, software         #
# distributed under the License is distributed on an "AS IS" BASIS,           #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
# See the License for the specific language governing permissions and         #
# limitations under the License.                                              #
###############################################################################

import logging
from client import OCCIclient

logging.basicConfig(
    filename='logFile.log',
    level=logging.INFO,
    format='%(asctime)s %(levelname)s - %(message)s',
    datefmt='%d/%m/%Y %H:%M:%S',
)

class gwProcci:


    def configure(self, cmd):
        cmd_conf = '/opt/vyatta/sbin/vyatta-cfg-cmd-wrapper '
        cmdConfig=    cmd_conf + 'begin &&' + cmd_conf + cmd + '&&' + cmd_conf + 'commit &&' + cmd_conf + 'end &&' + cmd_conf + 'save'
        return cmdConfig


    def load(self):
        return self.configure('load')





    '''
    DHCP Configuration
    '''



    def configure_DHCP(self, namePool, subnet, start, stop, gatewayAddress, dnsAddress):

        cmd_conf = '/opt/vyatta/sbin/vyatta-cfg-cmd-wrapper '
        return self.configure(
            'set service dhcp-server shared-network-name ' + namePool + ' subnet ' + subnet + ' start ' + start + ' stop ' + stop +
            '&&' + cmd_conf + 'set service dhcp-server shared-network-name ' + namePool + ' subnet ' + subnet + ' default-router ' + gatewayAddress +
            '&&' + cmd_conf + 'set service dhcp-server shared-network-name ' + namePool + ' subnet ' + subnet + ' dns-server ' + dnsAddress)



    def configure_DHCP_addrPool(self, namePool, subnet, start, stop):

        return self.configure(
            'set service dhcp-server shared-network-name ' + namePool + ' subnet ' + subnet + ' start ' + start + ' stop ' + stop)



    def configure_DHCP_defaultGW(self, namePool, subnet, gatewayAddress):

        return self.configure(
            'set service dhcp-server shared-network-name ' + namePool + ' subnet ' + subnet + ' default-router ' + gatewayAddress)



    def configure_DHCP_dnsServer(self, namePool, subnet, dnsAddress):

        return self.configure(
            'set service dhcp-server shared-network-name ' + namePool + ' subnet ' + subnet + ' dns-server ' + dnsAddress)







    '''
    NAT Configuration
    '''


    def configure_NAT(self, ruleNumb, address, outInterface, type):

        cmd_conf = '/opt/vyatta/sbin/vyatta-cfg-cmd-wrapper '
        return self.configure(
            'set service nat rule ' + ruleNumb + ' source address ' + address + '&&' + cmd_conf +
            'set service nat rule ' + ruleNumb + ' outbound-interface ' + outInterface + '&&' + cmd_conf +
            'set service nat rule ' + ruleNumb + ' type ' + type)






    def configure_Interface(self, interface, address):

        return self.configure('set interfaces ethernet ' + interface + ' address ' + address)


    def configure_DNSServer(self, dnsAddress):

        return self.configure('set system name-server ' + dnsAddress)



    def configure_DefaultGW(self, gatewayAddress):

        return self.configure('set system gateway-address ' + gatewayAddress)




    def configure_NameServer(self, nameServer):

        return self.configure('set system host-name ' + nameServer)






    '''
    GRE Configuration
    '''


    def configure_GRE(self, tunnelName, tunnelAddress, description, mode, localAddress, remoteAddress):

        cmd_conf = '/opt/vyatta/sbin/vyatta-cfg-cmd-wrapper '
        return self.configure(
            'set interfaces tunnel ' + tunnelName + ' address ' + tunnelAddress +
            '&&' + cmd_conf + 'set interfaces tunnel ' + tunnelName + ' description "' + description + '"' +
            '&&' + cmd_conf + 'set interfaces tunnel ' + tunnelName + ' encapsulation ' + mode +
            '&&' + cmd_conf + 'set interfaces tunnel ' + tunnelName + ' multicast enable ' +
            '&&' + cmd_conf + 'set interfaces tunnel ' + tunnelName + ' local-ip ' + localAddress +
            '&&' + cmd_conf + 'set interfaces tunnel ' + tunnelName + ' remote-ip ' + remoteAddress)


    def configure_Tunnel_interface(self, tunnelName, tunnelAddress):

        return self.configure('set interfaces tunnel ' + tunnelName + ' address ' + tunnelAddress)



    def configure_Tunnel_description(self, tunnelName, description):

        return self.configure('set interfaces tunnel ' + tunnelName + ' description "' + description + '"')


    def configure_Tunnel_encapsulationMode(self, tunnelName, mode):

        return self.configure('set interfaces tunnel ' + tunnelName + ' encapsulation ' + mode)



    def configure_Tunnel_multicast(self, tunnelName):

        return self.configure('set interfaces tunnel ' + tunnelName + ' multicast enable')



    def configure_Tunnel_localIP(self, tunnelName, localAddress):

        return self.configure('set interfaces tunnel ' + tunnelName + ' local-ip ' + localAddress)



    def configure_Tunnel_remoteIP(self, tunnelName, remoteAddress):

        return self.configure('set interfaces tunnel ' + tunnelName + ' remote-ip ' + remoteAddress)




    def configure_staticRoute(self, dstAddress, nextHop):
        return self.configure('set protocols static route ' + dstAddress + ' next-hop ' + nextHop)





    '''
    IPSEC Configuration
    '''



    def configure_IKE_proposal(self, ikeName, proposalNum, encryption, hash):

        cmd_conf = '/opt/vyatta/sbin/vyatta-cfg-cmd-wrapper '
        return self.configure(
            'set vpn ipsec ike-group ' + ikeName + ' proposal ' + proposalNum + ' encryption ' + encryption +
            '&&' + cmd_conf + 'set vpn ipsec ike-group ' + ikeName + ' proposal ' + proposalNum + ' hash ' + hash)



    def configure_Protocol_lifetime(self, protocol, Name, lifetime):

        return self.configure('set vpn ipsec ' + protocol + '-group ' + Name + ' lifetime ' + lifetime)




    def configure_ESP_proposal(self, espName, proposalNum, encryption, hash):

        cmd_conf = '/opt/vyatta/sbin/vyatta-cfg-cmd-wrapper '
        return self.configure(
            'set vpn ipsec esp-group ' + espName + ' proposal ' + proposalNum + ' encryption ' + encryption +
            '&&' + cmd_conf + 'set vpn ipsec esp-group ' + espName + ' proposal ' + proposalNum + ' hash ' + hash)



    def configure_ESP_lifetime(self, espName, lifetime):

        return self.configure('set vpn ipsec esp-group ' + espName + ' lifetime ' + lifetime)




    def configure_IPSEC_interface(self, interface):

        return self.configure('set vpn ipsec ipsec-interfaces interface ' + interface)





    '''
    site-to-site Configuration (Ipsec between two site)
    '''



    def configure_IPSEC_SiteToSite(self, remoteAddress, authenticationKey, esp, ike, localAddress, tunnelNum, protocol):

        cmd_conf = '/opt/vyatta/sbin/vyatta-cfg-cmd-wrapper '
        return  self.configure(
            'set vpn ipsec site-to-site peer ' + remoteAddress + ' authentication mode pre-shared-secret ' +
            '&&' + cmd_conf + 'edit vpn ipsec site-to-site peer ' + remoteAddress +
            '&&' + cmd_conf + 'set authentication pre-shared-secret ' + authenticationKey +
            '&&' + cmd_conf + 'set default-esp-group ' + esp +
            '&&' + cmd_conf + 'set ike-group ' + ike +
            '&&' + cmd_conf + 'set local-ip ' + localAddress +
            '&&' + cmd_conf + 'set tunnel ' + tunnelNum + ' protocol ' + protocol +
            '&&' + cmd_conf + 'top ')


    def configure_IPSEC_SiteToSite_withfile(self, remoteAddress, authenticationKey, esp, ike, localAddress, tunnelNum, protocol):

        return '/home/vyatta/addipsec '+remoteAddress+' '+authenticationKey+' '+esp+' '+ike+' '+localAddress+' '+tunnelNum+' '+protocol



    def configure_protocol_IPSEC(self, gwinstance, address, ethername, IKEname, ESPname):

        cmds=[]
        cmds.append(gwinstance.configure_IPSEC_interface(ethername))

        cmds.append(gwinstance.configure_IKE_proposal(IKEname, '1', 'aes256','sha1'))
        cmds.append(gwinstance.configure_IKE_proposal(IKEname, '2', 'aes128','sha1'))
        cmds.append(gwinstance.configure_Protocol_lifetime('ike', IKEname, '3600'))

        cmds.append(gwinstance.configure_ESP_proposal(ESPname, '1', 'aes256','sha1'))
        cmds.append(gwinstance.configure_ESP_proposal(ESPname, '2', '3des','md5'))
        cmds.append(gwinstance.configure_Protocol_lifetime('esp', ESPname, '1800'))
        gwinstance.execute_many_cmd_Cosacs(address, cmds)


    def connect_gw(self, gwinstance, greAdress, prefix, tunnelProtocol, localAddress, remoteAddress, authenticationKey, grenexthop, dstAddress):

        cmds=[]
        cmds.append(gwinstance.configure_GRE('tun0', greAdress+'/'+prefix, 'GRE description', tunnelProtocol, localAddress, remoteAddress))
        cmds.append(gwinstance.configure_IPSEC_SiteToSite_withfile(remoteAddress, authenticationKey, 'ESP', 'IKE', localAddress, '1' , tunnelProtocol))
        cmds.append(gwinstance.load())

        cmds.append(gwinstance.configure_staticRoute(dstAddress, grenexthop))

        gwinstance.execute_many_cmd_Cosacs(localAddress, cmds)




    def execute_many_cmd_Cosacs(self, address, cmds):
        for item in cmds:

            attributes = {
                'name': '',
                'syntax': item,
                'nature': 'system',
                'status': '',
                'identifier': ''
            }
            cl = OCCIclient(address, '8286', 'COSACS', 'script', attributes)
            cl.Post()

        cosacsStart = {
            'name': 'cosacs:start'
        }

        startcmd = OCCIclient(address, '8286', 'COSACS', 'script', cosacsStart)
        startcmd.Post()

