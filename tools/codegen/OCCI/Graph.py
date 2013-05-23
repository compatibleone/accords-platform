'''
Created on 2 Apr 2013

@author: Jonathan Custance
'''

import OCCI.Category
import OCCI.Output
import pydot
import logging

class Graph(OCCI.Output.Output):
    '''
    Generate a graph of model relationships
    '''       
        
    def go(self):
        '''
        Generate the graph and write it to a PNG.
        '''
        
        logging.info("Writing Graph")
        graph = pydot.Dot(graph_type='digraph', rankdir="LR")
        
        node_dict = {}
        edges = []
        
        # TODO consider option to restrict graph to specific models
        
        # Build all the nodes
        for model in self.models.list.values():
            for cat in model.list.values():
                field_list = []
                assert isinstance(cat, OCCI.Category.Category)
                count = 0
                # Build list of fields for this node
                for name in cat.attrs.keys():
                    field_list.append(name)
                    count = count+ 1        
                # Add any edges we may require
                for name, coll in cat.colls.items():
                    field_list.append(name)
                    source = '"'+cat.term+'":f'+str(count+1)
                    edges.append({"source":source, "target":'"'+coll.category.split("#")[1]+'":f0'})
                    count = count + 1 
                # Create the node label
                label = "<f0> "+cat.term
                count = 1
                for field in field_list:
                    label = label + "|" + "<f" + str(count)+"> "+field
                    count = count +1
                logging.info(label)
                # Add the node
                node = pydot.Node('"'+cat.term+'"', shape="record", label='"'+label+'"')
                node_dict[cat.catid] = node
                graph.add_node(node)
    
        # Add all the edges (now we have all the nodes added)
        for edge in edges:
            target = edge["target"]
            source = edge["source"]
            # We ignore account and price links - they are too numerous
            # TODO consider passing in args to ignore
            if target.count("account") == 0  and source.count("account") == 0 and target.count("price") == 0 and source.count("price") == 0:
                edge = pydot.Edge(edge["source"], edge["target"])
                graph.add_edge(edge)
        
        logging.info(edges)
        
        # and we are done
        logging.info("Writing graph to "+self.output_dir + 'graph.png')
        graph.write_png(self.output_dir + 'graph.png')
        return None