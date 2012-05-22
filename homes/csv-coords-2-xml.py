#!/usr/bin/python
import sys
import os

def main() :

	if len(sys.argv) < 2 :
		print >> sys.stderr, "No csv file was specified"
		sys.exit(1)

	with open(sys.argv[1]) as instream :

		for line in instream :
			line = line.strip()
			tokens = line.split(',')
			if '"' in tokens[0] :
				print >> sys.stdout, line
				continue
			xml_snip = '<Point x="%s" y="%s" />'%(tokens[0], tokens[1])
			print >> sys.stdout, xml_snip

if __name__ == '__main__' :
	main()
