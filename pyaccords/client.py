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

import httplib2
import pprint
import re

class OCCIclient:

    def __init__(self, host, port, userAgent, category, attributes):
        self.host = host
        self.port = port
        self.userAgent = userAgent
        self.category = category
        self.attributes = attributes


    def Post(self):
        h = httplib2.Http(".cache")
        msg = 'POST /' + self.category + '/ HTTP/1.1' \
              '\r\nHost: http:// ' + self.host + ':' + self.port + \
              '\r\nUser-Agent: ' + self.userAgent + '/1.0' \
              '\r\nCATEGORY: '+ self.category + \
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";' \
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";'
        for item in self.attributes:
            msg=msg + '\r\nX-OCCI-ATTRIBUTE: occi.'+ self.category +'.'+item+'='+self.attributes[item]
        msg=msg+'\r\n\r\n'

        resp, content = h.request('http://' + self.host + ':' + self.port + '/' + self.category + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')

        if(resp.has_key('x-occi-location')):
            return resp['x-occi-location'].replace(' ', '')
        else:
            return ''


    def GetElement(self, uuid):
        h = httplib2.Http(".cache")
        msg = 'GET /' + self.category + '/' + uuid + ' HTTP/1.1'\
              '\r\nHost: http:// ' + self.host + ':' + self.port +\
              '\r\nUser-Agent: ' + self.userAgent + '/1.0'\
              '\r\nCATEGORY: '+ self.category +\
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";'\
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";' + '\r\n\r\n'
        resp, content = h.request('http://' + self.host + ':' + self.port + '/' + self.category + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')

        listattribute = resp['x-occi-attribute'].replace(' ','').split(',')
        dictattribute = {}
        for i in range(len(listattribute)):
            dictattribute[listattribute[i][:listattribute[i].find('=')]]=listattribute[i][listattribute[i].find('=')+1:]

        return dictattribute

    def GetElement_pathuuid(self, pathuuid):
        parametres = {}
        templist = pathuuid.split()
        pathuuid = ''.join(templist)
        temppath = pathuuid[7:]
        parametres['host']=temppath[:temppath.find(':')]
        temppath = temppath[temppath.find(':')+1:]
        if (temppath.find('/') != -1):
            parametres['port']=temppath[:temppath.find('/')]
            temppath = temppath[temppath.find('/')+1:]
            parametres['category']=temppath[:temppath.find('/')]
            parametres['uuid']=temppath[temppath.find('/')+1:]
        else:
            parametres['port']=temppath
        return parametres


    def GetAttribute_category(self):
        h = httplib2.Http(".cache")
        msg = 'GET /-/ HTTP/1.1'\
              '\r\nHost: http:// ' + self.host + ':' + self.port +\
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";'\
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";' + '\r\n\r\n'

        resp, content = h.request('http://' + self.host + ':' + self.port + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')

        begin = 'attributes='
        end = ';'
        motif = begin + '.*' + end
        temptext=resp['category']
        obj_regex = re.search(motif, temptext)
        dictcategorie = {}
        while (obj_regex != None):
            text = obj_regex.group()
            text = text.replace(begin, '')
            text = text.replace(end, '')
            text = text.strip()
            temptext = temptext[temptext.find(begin) + len(begin):]
            obj_regex = re.search(motif, temptext)
            dictcategorie[text[5:5+text[5:].find('.')]] = text.split(',')

        return dictcategorie


    def Get(self):
        h = httplib2.Http(".cache")
        msg = 'GET /' + self.category + '/ HTTP/1.1'\
              '\r\nHost: http:// ' + self.host + ':' + self.port +\
              '\r\nUser-Agent: ' + self.userAgent + '/1.0'\
              '\r\nCATEGORY: '+ self.category +\
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";'\
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";'

        for item in self.attributes:
            msg=msg + '\r\nX-OCCI-ATTRIBUTE: occi.'+ self.category +'.'+item+'='+self.attributes[item]
        msg=msg+'\r\n\r\n'

        resp, content = h.request('http://' + self.host + ':' + self.port + '/' + self.category + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')

        if(resp.has_key('x-occi-location')):
            return resp['x-occi-location'].split(',')
        else:
            return ''


    def Delete(self):
        h = httplib2.Http(".cache")
        msg = 'DELETE /' + self.category + '/ HTTP/1.1'\
              '\r\nHost: http:// ' + self.host + ':' + self.port +\
              '\r\nUser-Agent: ' + self.userAgent + '/1.0'\
              '\r\nCATEGORY: '+ self.category +\
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";'\
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";'


        for item in self.attributes:
            msg=msg + '\r\nX-OCCI-ATTRIBUTE: occi.'+ self.category +'.'+item+'='+self.attributes[item]
        msg=msg+'\r\n\r\n'

        resp, content = h.request('http://' + self.host + ':' + self.port + '/' + self.category + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')


    def Put(self,uuid):
        h = httplib2.Http(".cache")
        msg = 'PUT /' + self.category + '/' + uuid + ' HTTP/1.1'\
              '\r\nHost: http:// ' + self.host + ':' + self.port +\
              '\r\nUser-Agent: ' + self.userAgent + '/1.0'\
              '\r\nCATEGORY: '+ self.category +\
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";'\
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";'

        for item in self.attributes:
            msg=msg + '\r\nX-OCCI-ATTRIBUTE: occi.'+ self.category +'.'+item+'='+self.attributes[item]
        msg=msg+'\r\n\r\n'

        resp, content = h.request('http://' + self.host + ':' + self.port + '/' + self.category + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')


    def action(self, uuid, action):
        h = httplib2.Http(".cache")
        msg = 'POST /' + self.category + '/' + uuid + '?action=' + action + ' HTTP/1.1'\
              '\r\nHost: http:// ' + self.host + ':' + self.port +\
              '\r\nUser-Agent: ' + self.userAgent + '/1.0'\
              '\r\nCATEGORY: '+ self.category +\
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";'\
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";' + '\r\n\r\n'
        resp, content = h.request('http://' + self.host + ':' + self.port + '/' + self.category + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')


    def GetLinks(self, uuid):
        h = httplib2.Http(".cache")
        msg = 'GET /' + self.category + '/' + uuid + ' HTTP/1.1'\
              '\r\nHost: http:// ' + self.host + ':' + self.port +\
              '\r\nUser-Agent: ' + self.userAgent + '/1.0'\
              '\r\nCATEGORY: '+ self.category +\
              ';\r\nscheme=\"http://scheme.compatibleone.fr/scheme/compatible#\";'\
              '\r\nclass=kind;\r\n rel=\"http://scheme.ogf.org/occi/resource#\";' + '\r\n\r\n'
        resp, content = h.request('http://' + self.host + ':' + self.port + '/' + self.category + '/',
            msg,
            headers={'content-type': 'text/html', 'accept': 'text/json'},
            body='')


        listlinks = resp['link'].replace(' ','').split(',')
        links = []
        for i in range(len(listlinks)):
            link = listlinks[i].replace(' ','').split(';')[0]
            links.append(link[1:len(link)-1])

        return links

