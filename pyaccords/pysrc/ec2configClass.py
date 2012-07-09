#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED (c) Prologue

class Cec2config:
	"""Class to define the ec2config category structure"""
	def __init__( self,name,description,accesskey,secretkey,authenticate,agent,host,version,namespace,base,tls,current ):
		"""Constructor of the class"""
		self.name = name
		self.description = description
		self.accesskey = accesskey
		self.secretkey = secretkey
		self.authenticate = authenticate
		self.agent = agent
		self.host = host
		self.version = version
		self.namespace = namespace
		self.base = base
		self.tls = tls
		self.current = current

