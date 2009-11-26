#!/usr/bin/env python

import MySQLdb, MySQLdb.cursors

class DB(object):
	def __init__(self):
		self.data = {}

	def addDatabases(self, dbs):
		for type, db in dbs.items():
			for name, info in db.items():
				self.addDatabase(type, name, info)

	def addDatabase(self, type, name, data):
		if type not in self.data:
			self.data[type] = {}
		
		self.data[type][name] = data
		
		self.setActive(type, name)

	def setActive(self, type, name):
		self.data[type]['active'] = self.data[type][name]
		
		self.prefix = self.data[type][name]['prefix']

	def query(self, query, type = "master", extra = None):
		result = None
		
		db = self.connect(type)
		
		if db:
			query = self.buildQuery(query, type)
			
			try:
				result = db.execute(query, extra)
			except (MySQLdb.Error, MySQLdb.OperationalError):
				pass
			
			sleep(0.1)
		
		return result

	def fetchRow(self, query, type = "master", extra = None):
		result = self.query(query, type, extra)
		
		if result:
			result = self.data[type]['active']['cursor'].fetchone()
		
		if not result:
			result = {}
		
		return result

	def fetchRows(self, query, type = "master", extra = None):
		result = self.query(query, type, extra)
		
		if result:
			"""
			If the query was actually successful, proceed fetching all results
			"""
			result = self.data[type]['active']['cursor'].fetchall()
		
		if not result:
			result = {}
		
		return result

	def buildQuery(self, query, type = "master"):
		if config.debugQueries:
			print "Executing query: ", query
		
		return query

	def connect(self, type):
		result = None
		
		if "active" not in self.data[type]:
			if "main" not in self.data[type]:
				print "error"
			else:
				self.data[type]['active'] = self.data[type]['main']
		
		db = self.data[type]['active']
		
		#self.disconnect(type)
		
		while not result:
			try:
				if db['link']:
					try:
						db['link'].ping() #test connection
						
						result = db['cursor']
					except (MySQLdb.Error, MySQLdb.OperationalError):
						db['link'] = None
				else:
					raise KeyError
			except (KeyError, IndexError):
				db['link'] = None #reset the link incase it holds an old connection
				
				try:
					db['link'] = MySQLdb.connect(db['host'], db['user'], db['pass'], db['db'])
					db['link'].ping() #test connection
					
					if db['link']:
						db['cursor'] = db['link'].cursor(MySQLdb.cursors.DictCursor)
						
						result = db['cursor']
				except (MySQLdb.Error, MySQLdb.OperationalError):
					pass
		
		if not result:
			print "Fatal Error: Could not connect to database."
		
		return result

	def disconnect(self, type):
		if "active" in self.data[type]:
			db = self.data[type]['active']
			
			if "cursor" in db and db['cursor']:
				db['cursor'].close()
				db['cursor'] = None
			
			if "link" in db and db['link']:
				db['link'].close()
				db['link'] = None
	
db = DB()