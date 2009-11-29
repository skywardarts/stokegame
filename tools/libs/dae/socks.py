################################################################################
# Python Standard Library
################################################################################

import socket, struct, time

from select import POLLIN, POLLPRI, POLLOUT, POLLERR, POLLHUP, POLLNVAL

################################################################################
# Includes
################################################################################

import debug
from ip import IP
from event import EventHandler

################################################################################
# Configuration
################################################################################

debugHex = False
debugBinary = False

################################################################################
# Packets
################################################################################

################################################################################
# Packet Handlers
################################################################################

class Packet(EventHandler):
	"""
	Packet
	"""
	
	def __init__(self, message = ''):
		self.message = message
		self.socket = None
	
	def unpack(self):
		debug.log <> "Unpacking packet..."
		
		self.dispatchEvent("unpack", (self,))
		
		debug.log <> "Packet unpacked."
	
	def pack(self):
		debug.log <> "Packing packet..."
		
		self.dispatchEvent("pack", (self,))
		
		debug.log <> "Packet packed."
		
		return self.message
	
################################################################################
# Socket Handlers
################################################################################

STREAM_PEEK = 2

READ_READY = 0
READ_COMPLETED = 1
WRITE_READY = 2
WRITE_COMPLETED = 3
ERROR_OCCURRED = 4
CONNECTION_REQUESTED = 5
CONNECTION_ESTABLISHED = 6
CONNECTION_FAILED = 7
CONNECTION_CLOSED = 8
TIMED_OUT = 9
CONNECT = 10
HANDSHAKE_SENT = 11
HANDSHAKE_COMPLETED = 12
HANDSHAKE_FAILED = 13

class network_socket_state(object):
	def __init__(self):
		self.binded = False
		self.listening = False
		self.reconnections = True

class network_socket(socket.socket):
	def __init__(self, sock = None):
		socket.socket.__init__(self, _sock = sock)
		
		self.socket = self
		
		self.socket.settimeout(5)
		
		self.source = IP()
		self.destination = IP()
		
		self.data = ''
		self.packets = []
		
		self.options = []
		
		self.binded = False
		
		self.states = network_socket_state()
		
		self.listeners = []
	
	def create(self):
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		
		self.socket.settimeout(5)
		
		self.applyOptions()
	
	def connect(self):
		if self.is_connected():
			return True
		else:
			try:
				if not self.socket:
					self.create()
				
				debug.log <> "Connecting to " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "..."
				
				socket.socket.connect(self.socket, (self.destination.host, self.destination.port))
			except socket.error:
				debug.log <> "Unable to connect to " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "."
				
				self.disconnect()
				
				return False
			else:
				debug.log <> "Connected to " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "."
				
				return True
	
	def is_connected(self):
		if self.socket:
			try:
				self.socket.getpeername()
			except socket.error, (number, message):
				print "errortastic"
				return False
			else:
				return True
		else:
			return False
	
	def applyOptions(self):
		for option in self.options:
			socket.socket.setsockopt(self.socket, *option)

	def bind(self):
		if not self.states.binded:
			if self.source.host is not None and self.source.port is not None:
				self.states.binded = True
				
				if not self.socket:
					self.create()
				
				socket.socket.setsockopt(self.socket, socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
				socket.socket.bind(self.socket, (self.source.host, self.source.port))
				
				debug.log <> "Binding connections on " + self.source.host + ":" + str(self.source.port) + "."
	
	def listen(self, seconds):
		if not self.states.listening:
			socket.socket.listen(self.socket, seconds)
			
			self.states.listening = True
			
			debug.log <> "Accepting connections on " + self.source.host + ":" + str(self.source.port) + "."
	
	def setsockopt(self, *options):
		self.options.append(options)
		
		if self.socket:
			socket.socket.setsockopt(self.socket, *options)
	
	def available(self):
		if self.is_connected():
			amount = 0
			
			checking = True
			
			while checking:
				temp = self.socket.recv(1024, 2)
				
				if len(temp) < 1024:
					checking = False
				else:
					time.sleep(0.05)
				
				amount += len(temp)

			return amount
		else:
			return -1
	
	def read(self, chars = 8092):
		data = []
		
		#if not self.isConnected():
		#	debug.log <> "Reconnecting to " + self.destination.host + ":" + str(self.destination.port) + "..."
		
		#if self.Connect():
		try:
			self.socket.setblocking(0)
			
		except socket.error:
			debug.log <> "Error unblocking " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "."
			
			self.disconnect()
		else:
			line = 'ready'
			
			while line:
				try:
					line = self.socket.recv(chars)
				except socket.error:
					if line != 'ready':
						line = ''
				else:
					data.append(line)
					
					if line.endswith("\r\n\r\n"):
						#only headers were sent, assume we're good to go
						line = ''
					elif len(line) == 8092:
						#we know theres more out there
						line = 'ready'
					elif len(line) == chars:
						#we got what we came for, so lets get outta here
						line = ''
					else:
						#stay a while and see if there's anymore
						time.sleep(0.5)
			
			self.socket.setblocking(1)
			
		#else:
		#	debug.log <> "Error reading from " + self.destination.host + ":" + str(self.destination.port) + "."
			
		#	self.Disconnect()
		
		data = ''.join(data)
		
		#if data:
			#debug.log <> "Receiving from " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "..."
			
			#if debugBinary:
			#	debug.log <> data + "\n"
			
			#if debugHex:
			#	debug.log <> ''.join(["\\x%02x" % ord(x) for x in data]).strip()
		
		return data

	def write(self, data):
		if self.is_connected():
			connected = True
		else:
			connected = self.reconnect()
		
		if connected:
			if data:
				#debug.log <> "Sending to " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "..."
				
				if debugBinary:
					debug.log <> data + "\n"
				
				if debugHex:
					debug.log <> ''.join(["\\x%02x" % ord( x ) for x in data]).strip()
				
				self.socket.sendall(data)
		else:
			debug.log <> "Error writing to " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "."
	
	def disconnect(self):
		self.shutdown()
		self.close()
	
	def reconnect(self):
		if self.states.reconnections:
			self.close()
			
			debug.log <> "Reconnecting to " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "..."
			
			return self.connect()
		else:
			debug.log <> "Reconnection to " + self.destination.host + ":" + str(self.destination.port) + " disabled on " + self.source.host + ":" + str(self.source.port) + "."
			
			return False
	
	def shutdown(self):
		try:
			socket.socket.shutdown(self.socket, socket.SHUT_RDWR)
		except socket.error:
			pass
		else:
			debug.log <> "Shutting down " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "."
	
	def close(self):
		try:
			socket.socket.close(self.socket)
		except socket.error:
			pass
		else:
			debug.log <> "Disconnected from " + self.destination.host + ":" + str(self.destination.port) + " on " + self.source.host + ":" + str(self.source.port) + "."
	
	def accept(self):
		sock, (host, port) = socket.socket.accept(self.socket)

		debug.log <> "Accepting connection from " + host + ":" + str(port) + " on " + self.source.host + ":" + str(self.source.port) + " ..."
		
		return sock, (host, port)

	def unpackPackets(self):
		i, l = 0, len(self.data)
		
		while i < l:
			debug.log <> "Unpacking packet..."
			
			packet = Packet()
			packet.messageLength = len(self.data)
			packet.message = self.data
			
			debug.log <> "Packet unpacked."
			
			self.packets.append(packet)
			
			i += packet.messageLength
		
		self.data = ''
	
	
	def lower_layer(self):
		return self.socket
	

