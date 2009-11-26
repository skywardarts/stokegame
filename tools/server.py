import socket, select, time, struct, random

from libs.dae.socks import network_socket

REALM_MESSAGE_KEEP_ALIVE = 1
REALM_MESSAGE_KEEP_ALIVE_OPEN = 1
REALM_MESSAGE_KEEP_ALIVE_CLOSE = 2

REALM_MESSAGE_USER_LOGIN = 2

GAME_MESSAGE_UPDATE_PLAYER_POSITION = 3

GAME_MESSAGE_CREATE_PLAYER = 5

GAME_MESSAGE_UPDATE_PLAYER = 6

GAME_MESSAGE_UPDATE_PLAYER_DIRECTION = 7

DEBUG1 = True

class vector2d(object):
	def __init__(self):
		self.x_ = 0
		self.y_ = 0
		self.changed = False
	
	def get_x(self):
		return self.x_

	def set_x(self, value):
		if self.x_ != value:
			self.x_ = value
			
			self.changed = True

	def get_y(self):
		return self.y_

	def set_y(self, value):
		if self.y_ != value:
			self.y_ = value
			
			self.changed = True

	x = property(get_x, set_x)
	y = property(get_y, set_y)


class player(object):
	def __init__(self, sock):
		self.socket = sock
		self.data = ""
		self.position = vector2d()
		self.id = 0
		self.name = "None"
		self.rotation = vector2d()

class policy_server(object):
	def __init__(self, port):
		self.read = []
		self.write = []
		self.error = []
		
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server.bind(('', port))
		self.server.listen(5)
		
		self.read.append(self.server)
	
	def update(self):
		r, w, e = select.select(self.read, self.write, self.error, 0)
		
		for sock in r:
			if sock == self.server:
				client, (host, port) = self.server.accept()
	
				if DEBUG1:
					print "[policy_server] Accepting client..."
	
				#read.append(client)
	
				self.write.append(client)
			else:
				available = sock.recv(1, 2)
				
				if available:
					data = sock.recv(1024)
					
					if data.startswith("<policy-file-request/>"):
						if DEBUG1:
							print "[policy_server] Accepted client."
						
						sock.send("<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"6110-6112\" /></cross-domain-policy>\0")
					else:
						if DEBUG1:
							print "[policy_server] Refused client."
		
		for sock in w:
			self.read.append(sock)
			self.write.remove(sock)

class game_server(object):
	def __init__(self, port):
		self.read = []
		self.write = []
		self.error = []
		
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server.bind(('', port))
		self.server.listen(5)
		
		self.read.append(self.server)
	
	def update(self):
		r, w, e = select.select(self.read, self.write, self.error, 0)
		
		for sock in r:
			if sock == self.server:
				client, (host, port) = self.server.accept()
	
				if DEBUG1:
					print "[game_server] Accepting client..."
	
				#read.append(client)
	
				self.write.append(client)
			else:
				pass
		
		for sock in w:
			self.read.append(sock)
			self.write.remove(sock)
	
class realm_server(object):
	def __init__(self, port):
		self.read = []
		self.write = []
		self.error = []
		
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server.bind(('', port))
		self.server.listen(5)
		
		self.player_list = {}

		self.timer = int(time.time() * 100)
		
		self.read.append(self.server)
	
	def update(self):
		r, w, e = select.select(self.read, self.write, self.error, 0)
	
		for sock in e:
			self.player_list.remove(sock)
			self.write.remove(sock)
			self.read.remove(sock)
	
		for sock in r:
			if sock == self.server:
				client, (host, port) = self.server.accept()
	
				if DEBUG1:
					print "[realm_server] Accepting client..."
	
				self.write.append(client)
	
				p = player(network_socket(client))
				p.id = random.randint(10000, 99999)
	
				self.player_list[client] = p
			else:
				p = self.player_list[sock]

				if p.socket.is_connected():
					available = p.socket.available()
					
					if available >= 8:
						data = p.socket.read(8)

						messageID = struct.unpack(">i", data[:4])[0]
						messageLength = struct.unpack(">i", data[4:8])[0]
						
						if available >= messageLength:
							messageData = p.socket.read(messageLength)
							
							if DEBUG1:
								print "messageID: " + str(messageID)
							if DEBUG1:
								print "messageLength: " + str(messageLength)

							if messageID == REALM_MESSAGE_KEEP_ALIVE:
								if DEBUG1:
									print "1) keepalive"
			
								msg = struct.pack(">i", REALM_MESSAGE_KEEP_ALIVE_OPEN)
			
								packet = struct.pack(">ii", REALM_MESSAGE_KEEP_ALIVE, len(msg))
								
								packet += msg
			
								p.socket.write(packet)
							
							elif messageID == REALM_MESSAGE_USER_LOGIN:
								if DEBUG1:
									print "2) login"

								# update the new player for old players
								msg = struct.pack(">ii" + str(len(p.name) * 4) + "siiiiB", p.id, len(p.name) * 4, p.name, p.position.x, p.position.y, 0, 0, False)
	
								new_player_packet = struct.pack(">ii", GAME_MESSAGE_CREATE_PLAYER, len(msg))
								new_player_packet += msg

								for sock in self.player_list:
									p2 = self.player_list[sock]

									if p2 == p:
										# update the new player for itself
										msg = struct.pack(">ii" + str(len(p.name) * 4) + "siiiiB", p.id, len(p.name) * 4, p.name, p.position.x, p.position.y, 0, 0, True)
	
										packet = struct.pack(">ii", GAME_MESSAGE_CREATE_PLAYER, len(msg))
										packet += msg

										p.socket.write(packet)
									else:
										p2.socket.write(new_player_packet)
		
										# show the old players for the new player
										msg = struct.pack(">ii" + str(len(p2.name) * 4) + "siiiiB", p2.id, len(p2.name) * 4, p2.name, p2.position.x, p2.position.y, 0, 0, False)
		
										packet = struct.pack(">ii", GAME_MESSAGE_CREATE_PLAYER, len(msg))
										packet += msg
				
										p.socket.write(packet)
			
							elif messageID == GAME_MESSAGE_UPDATE_PLAYER_DIRECTION:
								if DEBUG1:
									print "3) move"
			
								p.rotation.x, p.rotation.y = struct.unpack(">ii", messageData[:8])
				else:
					del self.player_list[sock]
					self.read.remove(sock)
	
	
		for sock in w:
			self.read.append(sock)
			self.write.remove(sock)
			
		timer = int(time.time() * 100)

		step = timer - self.timer

		self.timer = timer

		
		
		for sock in self.player_list:
			p = self.player_list[sock]

			if p.rotation.x == 1:
				p.position.x += step
			elif p.rotation.x == -1:
				p.position.x -= step
			elif p.rotation.y == 1:
				p.position.y += step
			elif p.rotation.y == -1:
				p.position.y -= step

			if p.position.changed:
				if DEBUG1:
					print "player id: " + str(p.id)
				if DEBUG1:
					print "player dir: " + str(p.rotation.x) + "/" + str(p.rotation.y)
				if DEBUG1:
					print "player pos: " + str(p.position.x) + "/" + str(p.position.y)

				if DEBUG1:
					print "changed pos"
				
				msg = struct.pack(">iii", p.id, p.position.x, p.position.y)
				packet = struct.pack(">ii", GAME_MESSAGE_UPDATE_PLAYER_POSITION, len(msg))
				packet += msg
	
				for sock2 in self.player_list:
					p2 = self.player_list[sock2]

					if p2 != p: # dont need to update own character's position
						p2.socket.write(packet)
				
				p.position.changed = False

class chat_server(object):
	def __init__(self, port):
		self.read = []
		self.write = []
		self.error = []
		
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server.bind(('', port))
		self.server.listen(5)
		
		self.read.append(self.server)
	
	def update(self):
		r, w, e = select.select(self.read, self.write, self.error, 0)
		
		for sock in r:
			if sock == self.server:
				client, (host, port) = self.server.accept()
	
				if DEBUG1:
					print "[chat_server] Accepting client..."
	
				#read.append(client)
	
				self.write.append(client)
			else:
				pass
		
		for sock in w:
			self.read.append(sock)
			self.write.remove(sock)

s1 = policy_server(843)
s2 = realm_server(6110)
s3 = chat_server(6112)
s4 = game_server(6111)

while True:
	s1.update()
	time.sleep(0.05)
	
	s2.update()
	time.sleep(0.05)
	
	s3.update()
	time.sleep(0.05)
	
	s4.update()
	time.sleep(0.05)


########################


