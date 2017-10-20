import _jetsonGPIO
import asyncio
import select

'''
A module for handling GPIO on the NVidia Jetson TX1
Mimics RPi.GPIO
'''

# Define constants
HIGH = 1
LOW = 0
IN = 0
OUT = 1
PUD_UP = 1
PUD_DOWN = 0
RISING = 'rising'
FALLING = 'falling'
BOTH = 'both'
_ACTIVE_PINS = []
_PIN_MODES = {}
_ACTIVE_COROUTINES = {} # maps pins to coroutine objects
_EVENT_CALLBACKS = {} # maps file descriptors to associated callbacks
_PIN_FILES = {} # maps file descriptors to pins 
_POLL_ROUTINE = None

poller = select.poll()

def setup(pin, mode):
    '''
    Setup a pin for GPIO.
    Valid pin values: 36, 37, 38, 63, 184, 186, 187, 219
    Valid modes: IN, OUT
    '''
    global _PIN_MODES
    global _ACTIVE_PINS
    
    # Export pin to user space
    _jetsonGPIO.gpioExport(pin)

    print ('hi')

    # Set pin direction
    _jetsonGPIO.gpioSetDirection(pin, mode)

    print ('hi')

    _ACTIVE_PINS.append(pin)
    _PIN_MODES[pin] = mode


def cleanup():
    '''
    Unexport and remove all active pins.
    '''
    global _PIN_MODES
    global _ACTIVE_PINS

    # Cancel polling
    if _POLL_ROUTINE:
        _POLL_ROUTINE.cancel()
    
    # Unexport pins from user space
    for pin in _ACTIVE_PINS:
        _jetsonGPIO.gpioUnexport(pin)

    _PIN_MODES = {}
    _ACTIVE_PINS = []

    raise

def _check_if_valid_pin(pin, mode):
    '''
    Check if the given pin is valid for making changes
    '''
    global _ACTIVE_PINS
    global _PIN_MODES
    if pin not in _ACTIVE_PINS:
        raise ValueError('Pin %s is not yet setup.' % pin)
    if _PIN_MODES[pin] != mode:
        if mode == IN:
            raise ValueError('Pin %s is not an input pin.' % pin)
        else:
            raise ValueError('Pin %s is not an output pin.' % pin)
    return True

def output(pin, value):
    '''
    Set an output pin's value to the specified value.
    Valid pin values: 36, 37, 38, 63, 184, 186, 187, 219
    Valid values: HIGH, LOW
    '''
    _check_if_valid_pin(pin, OUT)
    _jetsonGPIO.gpioSetValue(pin, value)
    # Get the file descriptor for the pin
    fd = _jetsonGPIO.gpioOpen(pin)

def input(pin):
    '''
    Read a pin's valuepin
    Valid pin values: 36, 37, 38, 63, 184, 186, 187, 219
    '''
    _check_if_valid_pin(pin, IN)
    _jetsonGPIO.gpioGetValue(pin)

def schedule_coroutine(target, loop=None):
    """Schedules target coroutine in the given event loop

    If not given, *loop* defaults to the current thread's event loop

    Returns the scheduled task.
    """
    if asyncio.iscoroutine(target):
        return asyncio.ensure_future(target, loop=loop)
    raise TypeError("target must be a coroutine, "
                    "not {!r}".format(type(target)))

def get_globals_for_poll():
    '''
    Get the latest values of the global variables and send to poll
    This may not be necessary as objects should be passed as references
    '''
    global poller 
    global _EVENT_CALLBACKS
    global _PIN_FILES
    return (poller, _EVENT_CALLBACKS, _PIN_FILES)

async def poll_for_events():
    '''
    Poll the active file descriptors for interrupts
    '''
    while True:
        (poller, _EVENT_CALLBACKS, _PIN_FILES) = get_globals_for_poll()
        events = poller.poll(10) # poll for 10 ms
        if events:
            for event in event:
                # get the file descriptor
                fd = event[0]
                # call the associated callback with the proper pin as arg
                _EVENT_CALLBACKS[fd](_PIN_FILES[fd])
        await asyncio.sleep(1)
        
def add_event_detect(pin, edge, callback):
    '''
    Add an event detection on a pin that will call the callback any time
    the edge interrupt is detected on the given pin.
    '''
    global poller 
    global _EVENT_CALLBACKS
    global _PIN_FILES

    _check_if_valid_pin(pin, IN)
    # What if the pin is already set with an event?
    _jetsonGPIO.gpioSetEdge(pin, edge)
    # Get the file descriptor for the pin
    fd = _jetsonGPIO.gpioOpen(pin) # maybe could get fd from setEdge
    _EVENT_CALLBACKS[fd] = callback
    _PIN_FILES[fd] = pin
    poller.register(fd)
    
    if not _POLL_ROUTINE:
        _POLL_ROUTINE = schedule_coroutine(poll_for_events)




