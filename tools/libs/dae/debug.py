#!/usr/bin/env python

import sys, datetime, logging


class Log:
	def __init__(self):
		self.messages = []
		
		###try:
		#	self.log = open(sys.path[0] + "\\logs\\" + str(datetime.datetime.today().replace(microsecond = 0)) + ".log", "w")
		#except IOError:
		#	self.log = False
			

		#	print "Unable to create log."

	def __del__(self):
		""""""
		#if self.log:
		#	self.log.close()
	
	def __ne__(self, message):
		#try:
		#	if self.log:
		#		self.log.write(message + "\n")
		#		self.log.flush()
		#except IOError:
		#	print "Error writing to disk."
			
		print message

log = Log()
