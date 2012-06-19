#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED (c) Prologue

class CamazonEc2:
	"""Class to define the amazonEc2 category structure"""
	def __init__( self,name,flavor,image,original,profile,node,price,account,number,rootpass,reference,network,access,accessip,floating,floatingid,publicaddr,privateaddr,firewall,group,zone,hostname,workload,when,state ):
		"""Constructor of the class"""
		self.name = name
		self.flavor = flavor
		self.image = image
		self.original = original
		self.profile = profile
		self.node = node
		self.price = price
		self.account = account
		self.number = number
		self.rootpass = rootpass
		self.reference = reference
		self.network = network
		self.access = access
		self.accessip = accessip
		self.floating = floating
		self.floatingid = floatingid
		self.publicaddr = publicaddr
		self.privateaddr = privateaddr
		self.firewall = firewall
		self.group = group
		self.zone = zone
		self.hostname = hostname
		self.workload = workload
		self.when = when
		self.state = state

