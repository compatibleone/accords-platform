##############################################################################
#copyright 2013, Hamid MEDJAHED  (hmedjahed@prologue.fr) Prologue            #
#Licensed under the Apache License, Version 2.0 (the "License");             #
#you may not use this file except in compliance with the License.            #
#You may obtain a copy of the License at                                     #
#                                                                            #
#       http://www.apache.org/licenses/LICENSE-2.0                           #
#                                                                            #
#Unless required by applicable law or agreed to in writing, software         #
#distributed under the License is distributed on an "AS IS" BASIS,           #
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
#See the License for the specific language governing permissions and         #
#limitations under the License.                                              #
##############################################################################

import HTMLParser

data = '''
<table cellspacing="0" class="table table-bordered table-hover table-condensed" id="data">
      <thead>
        <tr>
          <th class="name">Name</th>
          <th class="memory">Memory</th>
          <th class="computeunits">
            <abbr title="One EC2 Compute Unit provides the equivalent CPU capacity of a 1.0-1.2 GHz 2007 Opteron or 2007 Xeon processor.">Compute Units</abbr>
          </th>
          <th class="storage">Storage</th>
          <th class="architecture">Architecture</th>
          <th class="ioperf">I/O Performance</th>
          <th class="maxips">
            <abbr title="Adding additional IPs requires launching the instance in a VPC.">Max IPs</abbr>
          </th>
          <th class="apiname">API Name</th>
          <th class="cost">Linux cost</th>
          <th class="cost">Windows cost</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="name">M1 Small</td>
          <td class="memory"><span sort="1.7">1.70 GB</span></td>
          <td class="computeunits"><span sort="1">1</span></td>
          <td class="storage"><span sort="160">160 GB</span></td>
          <td class="architecture">32/64-bit</td>
          <td class="ioperf"><span sort="1">Moderate</span></td>
          <td class="maxips">8</td>
          <td class="apiname">m1.small</td>
          <td class="cost" hour_cost="0.060">$0.060 per hour</td>
          <td class="cost" hour_cost="0.115">$0.115 per hour</td>
        </tr>
        <tr>
          <td class="name">M1 Medium</td>
          <td class="memory"><span sort="3.75">3.75 GB</span></td>
          <td class="computeunits"><span sort="2">2</span></td>
          <td class="storage"><span sort="410">410 GB</span></td>
          <td class="architecture">32/64-bit</td>
          <td class="ioperf"><span sort="1">Moderate</span></td>
          <td class="maxips">12</td>
          <td class="apiname">m1.medium</td>
          <td class="cost" hour_cost="0.12">$0.12 per hour</td>
          <td class="cost" hour_cost="0.23">$0.23 per hour</td>
        </tr>
        <tr>
          <td class="name">M1 Large</td>
          <td class="memory"><span sort="7.5">7.50 GB</span></td>
          <td class="computeunits"><span sort="4">4</span></td>
          <td class="storage"><span sort="850">850 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="3">High</span></td>
          <td class="maxips">30</td>
          <td class="apiname">m1.large</td>
          <td class="cost" hour_cost="0.24">$0.24 per hour</td>
          <td class="cost" hour_cost="0.46">$0.46 per hour</td>
        </tr>
        <tr>
          <td class="name">M1 Extra Large</td>
          <td class="memory"><span sort="15">15.00 GB</span></td>
          <td class="computeunits"><span sort="8">8</span></td>
          <td class="storage"><span sort="1690">1690 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="3">High</span></td>
          <td class="maxips">60</td>
          <td class="apiname">m1.xlarge</td>
          <td class="cost" hour_cost="0.48">$0.48 per hour</td>
          <td class="cost" hour_cost="0.92">$0.92 per hour</td>
        </tr>
        <tr>
          <td class="name">Micro</td>
          <td class="memory"><span sort="0.6">0.60 GB</span></td>
          <td class="computeunits"><span sort="2">2</span></td>
          <td class="storage"><span sort="0">0 GB</span></td>
          <td class="architecture">32/64-bit</td>
          <td class="ioperf"><span sort="0">Low</span></td>
          <td class="maxips">1</td>
          <td class="apiname">t1.micro</td>
          <td class="cost" hour_cost="0.02">$0.02 per hour</td>
          <td class="cost" hour_cost="0.02">$0.02 per hour</td>
        </tr>
        <tr>
          <td class="name">High-Memory Extra Large</td>
          <td class="memory"><span sort="17.10">17.10 GB</span></td>
          <td class="computeunits"><span sort="6.5">6.5</span></td>
          <td class="storage"><span sort="420">420 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="1">Moderate</span></td>
          <td class="maxips">60</td>
          <td class="apiname">m2.xlarge</td>
          <td class="cost" hour_cost="0.41">$0.41 per hour</td>
          <td class="cost" hour_cost="0.57">$0.57 per hour</td>
        </tr>
        <tr>
          <td class="name">High-Memory Double Extra Large</td>
          <td class="memory"><span sort="34.2">34.20 GB</span></td>
          <td class="computeunits"><span sort="13">13</span></td>
          <td class="storage"><span sort="850">850 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="2">High</span></td>
          <td class="maxips">120</td>
          <td class="apiname">m2.2xlarge</td>
          <td class="cost" hour_cost="0.82">$0.82 per hour</td>
          <td class="cost" hour_cost="1.14">$1.14 per hour</td>
        </tr>
        <tr>
          <td class="name">High-Memory Quadruple Extra Large</td>
          <td class="memory"><span sort="68.4">68.40 GB</span></td>
          <td class="computeunits"><span sort="26">26</span></td>
          <td class="storage"><span sort="1690">1690 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="3">High</span></td>
          <td class="maxips">240</td>
          <td class="apiname">m2.4xlarge</td>
          <td class="cost" hour_cost="1.64">$1.64 per hour</td>
          <td class="cost" hour_cost="2.28">$2.28 per hour</td>
        </tr>
        <tr>
          <td class="name">M3 Extra Large</td>
          <td class="memory"><span sort="15">15.00 GB</span></td>
          <td class="computeunits"><span sort="13">13</span></td>
          <td class="storage"><span sort="0">0 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="1">Moderate</span></td>
          <td class="maxips">60</td>
          <td class="apiname">m3.xlarge</td>
          <td class="cost" hour_cost="0.50">$0.50 per hour</td>
          <td class="cost" hour_cost="0.98">$0.98 per hour</td>
        </tr>
        <tr>
          <td class="name">M3 Double Extra Large</td>
          <td class="memory"><span sort="30">30.00 GB</span></td>
          <td class="computeunits"><span sort="26">26</span></td>
          <td class="storage"><span sort="0">0 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="2">High</span></td>
          <td class="maxips">120</td>
          <td class="apiname">m3.2xlarge</td>
          <td class="cost" hour_cost="1.00">$1.00 per hour</td>
          <td class="cost" hour_cost="1.96">$1.96 per hour</td>
        </tr>
        <tr>
          <td class="name">High-CPU Medium</td>
          <td class="memory"><span sort="1.7">1.70 GB</span></td>
          <td class="computeunits"><span sort="5">5</span></td>
          <td class="storage"><span sort="350">350 GB</span></td>
          <td class="architecture">32_64-bit</td>
          <td class="ioperf"><span sort="1">Moderate</span></td>
          <td class="maxips">12</td>
          <td class="apiname">c1.medium</td>
          <td class="cost" hour_cost="0.145">$0.145 per hour</td>
          <td class="cost" hour_cost="0.285">$0.285 per hour</td>
        </tr>
        <tr>
          <td class="name">High-CPU Extra Large</td>
          <td class="memory"><span sort="7">7.00 GB</span></td>
          <td class="computeunits"><span sort="20">20</span></td>
          <td class="storage"><span sort="1690">1690 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="2">High</span></td>
          <td class="maxips">60</td>
          <td class="apiname">c1.xlarge</td>
          <td class="cost" hour_cost="0.58">$0.58 per hour</td>
          <td class="cost" hour_cost="1.14">$1.14 per hour</td>
        </tr>
        <tr>
          <td class="name">Cluster Compute Quadruple Extra Large</td>
          <td class="memory"><span sort="23">23.00 GB</span></td>
          <td class="computeunits"><span sort="33.5">33.5</span></td>
          <td class="storage"><span sort="1690">1690 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="4">Very High</sort></td>
          <td class="maxips">1</td>
          <td class="apiname">cc1.4xlarge</td>
          <td class="cost" hour_cost="1.30">$1.30 per hour</td>
          <td class="cost" hour_cost="1.61">$1.61 per hour</td>
        </tr>
        <tr>
          <td class="name">Cluster Compute Eight Extra Large</td>
          <td class="memory"><span sort="60.5">60.50 GB</span></td>
          <td class="computeunits"><span sort="88">88</span></td>
          <td class="storage"><span sort="3370">3370 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="4">Very High</sort></td>
          <td class="maxips">240</td>
          <td class="apiname">cc2.8xlarge</td>
          <td class="cost" hour_cost="2.40">$2.40 per hour</td>
          <td class="cost" hour_cost="2.97">$2.97 per hour</td>
        </tr>
        <tr>
          <td class="name">Cluster GPU Quadruple Extra Large</td>
          <td class="memory"><span sort="22">22.00 GB</span></td>
          <td class="computeunits"><span sort="33.5">33.5</span></td>
          <td class="storage"><span sort="1690">1690 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="4">Very High</sort></td>
          <td class="maxips">1</td>
          <td class="apiname">cg1.4xlarge</td>
          <td class="cost" hour_cost="2.10">$2.10 per hour</td>
          <td class="cost" hour_cost="2.60">$2.60 per hour</td>
        </tr>
        <tr>
          <td class="name">High I/O Quadruple Extra Large</td>
          <td class="memory"><span sort="60.5">60.50 GB</span></td>
          <td class="computeunits"><span sort="35">35</span></td>
          <td class="storage"><span sort="2048">2048 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="4">Very High</sort></td>
          <td class="maxips">1</td>
          <td class="apiname">hi1.4xlarge</td>
          <td class="cost" hour_cost="3.10">$3.10 per hour</td>
          <td class="cost" hour_cost="3.58">$3.58 per hour</td>
        </tr>
        <tr>
          <td class="name">High Storage Eight Extra Large</td>
          <td class="memory"><span sort="117.00">117.00 GB</span></td>
          <td class="computeunits"><span sort="35">35</span></td>
          <td class="storage"><span sort="49152">48 TB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="4">Very High</sort></td>
          <td class="maxips">1</td>
          <td class="apiname">hs1.8xlarge</td>
          <td class="cost" hour_cost="4.600">$4.600 per hour</td>
          <td class="cost" hour_cost="4.931">$4.931 per hour</td>
        </tr>
        <tr>
          <td class="name">High Memory Cluster Eight Extra Large</td>
          <td class="memory"><span sort="244.00">244.00 GB</span></td>
          <td class="computeunits"><span sort="88">88</span></td>
          <td class="storage"><span sort="240">240 GB</span></td>
          <td class="architecture">64-bit</td>
          <td class="ioperf"><span sort="4">Very High</sort></td>
          <td class="maxips">1</td>
          <td class="apiname">cr1.8xlarge</td>
          <td class="cost" hour_cost="3.500">$3.500 per hour</td>
          <td class="cost" hour_cost="3.831">$3.831 per hour</td>
        </tr>
      </tbody>
    </table> '''


class TableParser(HTMLParser.HTMLParser):
	def __init__(self):
		HTMLParser.HTMLParser.__init__(self)
		self.in_td = False
		self.flavors = []

	def handle_starttag(self, tag, attrs):
		if tag == 'td':
			self.in_td = True

	def handle_data(self, data):
		if self.in_td:
			self.flavors.append(data)

	def handle_endtag(self, tag):
		self.in_td = False

