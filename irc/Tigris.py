#!/usr/bin/env python

import asyncore, socket, time, re
import IRCParser

class Tigris(asyncore.dispatcher):
	def __init__(self, address, port, eventhandler):
		asyncore.dispatcher.__init__(self)
		self.event = eventhandler
		self.output = []
		self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
		self.connect( (address, port) )
		self.registered = False
		self.joined = False
		self.recvbuf = ''
		self.irc = IRCParser.IRCParser()
		self.sendlog = open('sendlog','w')

	def writeLine(self, line):
		self.output.append(line+'\r\n')
	
	def writeLines(self, lines):
		self.output.extend(lines)

	def handle_connect(self):
		self.writeLine('NICK Tigris')
		time.sleep(5)
		self.writeLine('USER Tigris 0 0 :Tigris (Python)')

	def handle_close(self):
		self.close()

	def handle_read(self):
		stuff = self.recv(1024)
		if stuff == '':
			# Connection closed at remote end
			self.close()

		# Split stuff into lines
		self.recvbuf += stuff
		newlines = self.recvbuf.split('\r\n')
		recvbuf = newlines.pop()
	
		for line in newlines:
			if not self.registered:
				if self.irc.searchNumeric('RPL_WELCOME', line):
					print '[Tigris Core] Registered on IRC'
					self.registered = True
	
			if self.registered and not self.joined:
				print '[Tigris Core] Joining #lobby'
				self.writeLine('JOIN #lobby')
				self.joined = True
	
			pingedby = self.irc.searchPing(line)
			if pingedby:
				print '[Tigris Core] Pinged by ' + pingedby
				self.writeLine('PONG ' + pingedby)
			else:
				self.event(self, line)

	def handle_write(self):
		while len(self.output):
			line = self.output.pop(0)
			self.doWrite(line)
			time.sleep(0.2)

	def doWrite(self, line):
		sent = 0
		while sent < len(line):
			line = line[sent:]
			sent = self.send(line)
			print '*** LINE LENGTH ' + str(len(line)) + ', SENT ' + str(sent) + ' ***'
			self.sendlog.write(line)
			self.sendlog.flush()



def PrintEventHandler(tigris, line):
	print line

tigris = Tigris('irc.some.server.here', 6667, PrintEventHandler)
asyncore.loop()
