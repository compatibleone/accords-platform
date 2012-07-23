#!/usr/bin/env python
# -*- coding: latin-1 -*-
# Hamid MEDJAHED & Elyes Zekri (c) Prologue

import sys
import pycompdev

def main():
	argc=len(sys.argv)
	return pycompdev.launchModule( argc , sys.argv , "ec2procci" ,"amazonEc2 ec2config")
if __name__=="__main__":
	main()
