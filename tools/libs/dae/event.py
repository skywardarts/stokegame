#!/usr/bin/env python

class EventHandler():
	events = {}
	
	def addEventListener(event, listener):
		if not event in e.events:
			e.events[event] = []
		
		e.events[event].append(listener)
	
	addEventListener = staticmethod(addEventListener)
	
	def removeEventListener(self, event, listener):
		for l, a in self.events[event]:
			if l is listener:
				self.remove((l, a))
	
	def addTimeout(self, timeout, delay):
		self.eventManager.addTimeout(timeout, delay)
	
	def dispatchEvent(event, arguments = ()):
		for listener in e.events[event]:
			listener(*arguments)
	
	dispatchEvent = staticmethod(dispatchEvent)

e = EventHandler()


class EventDispatcher:
	def __init__(self, *args):
		self.__events__ = args
	
	def __getattr__(self, name):
		assert name in self.__events__, "Event '%s' is not declared" % name
		
		self.__dict__[name] = ev = EventSlot(name)
		
		return ev
	
	def __repr__(self): return 'Events' + str(list(self))
	
	__str__ = __repr__
	
	def __len__(self): return NotImplemented
	
	def __iter__(self):
		def gen(dictitems=self.__dict__.items()):
			for attr, val in dictitems:
				if isinstance(val, EventSlot):
					yield val
		
		return gen()

class EventSlot:
	def __init__(self, name):
		self.targets = []
		self.__name__ = name
	
	def __repr__(self):
		return 'event ' + self.__name__

	def __call__(self, *a, **kw):
		for f in self.targets: f(*a, **kw)
	
	def __iadd__(self, f):
		self.targets.append(f)
		
		return self

	def __isub__(self, f):
		while f in self.targets: self.targets.remove(f)
		
		return self