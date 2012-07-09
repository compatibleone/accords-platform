#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED (c) Prologue

class respAction:
	"""Class to define the action response category structure"""
	def __init__( self,status,message):
		"""Constructor of the class"""
		self.status = status
		self.message = message

