from bottle import route, post, run, template, static_file
from threading import RLock, Lock
import json
import logging

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger()

class Display(object):

    g_disp_id = 0
    g_lock = Lock()

    @staticmethod
    def CreateDisplay(owner, name, width, height, bpp, primary):
        with Display.g_lock:
            Display.g_disp_id += 1
            return Display(owner, Display.g_disp_id, name, width, height, bpp, primary)

    def __init__(self, owner, disp_id, name, width, height, bpp, primary):
        self.name = name
        self.width = width
        self.height = height
        self.bpp = bpp 
        self.disp_id = disp_id
        self.primary = primary
        self.enabled = True
        self.owner = owner

    def modify(self, **kwargs):
        logger.debug("Modifying display (%d)", self.disp_id)
        modified = False
        with self.owner.lock:
            for key, value in kwargs.iteritems():
                if key == "enabled":
                    self.enabled = value
                    modified = True
                elif key == "width":
                    self.width = value
                    modified = True
                elif key == "height":
                    self.height = value
                    modified = True
                elif key == "bpp":
                    self.bpp = value
                    modified = True
                else:
                    logger.error("Invalid key (%s)", key)
                    raise Exception("Invalid display key!")
            if modified:
                self.owner.increment()
            return modified

    def to_dict(self):
        return {'disp_id': self.disp_id,
                'name': self.name,
                'width': self.width,
                'height': self.height,
                'enabled': self.enabled,
                'primary': self.primary,
                'bpp': self.bpp}

    def get_update(self):
        pass

class DisplayGroup(object):

    g_gid = 0
    g_lock = Lock()

    @staticmethod
    def CreateGroup(owner):
        with DisplayGroup.g_lock:
            DisplayGroup.g_gid += 1
            return DisplayGroup(owner, DisplayGroup.g_gid)

    def __init__(self, owner, gid):
        assert(isinstance(owner, Topology))
        self.gid = gid
        self.owner = owner
        self.displays = []

    def add_display(self, display):
        assert(isinstance(display, Display))
        with self.owner.lock:
            self.displays.append(display)
            self.owner.increment()

    def to_dict(self):
        disp_list = []
        for display in self.displays:
            disp_list.append(display.to_dict())
        return {'displays': disp_list}


class Topology(object):
    _inst = None
    @staticmethod
    def GetInstance():
        if Topology._inst is None:
            Topology._inst = Topology()
        return Topology._inst

    def __init__(self):
        assert(Topology._inst is None)
        self.display_groups = []
        self.tid = 0
        self.lock = RLock()
        self.update_pending = False

    def increment(self):
        with self.lock:
            self.tid += 1
            self.update_pending = True

    def add_group(self, group):
        assert(isinstance(group, DisplayGroup))
        with self.lock:
            self.display_groups.append(group)
            self.increment()

    def remove_group(self, group):
        assert(isinstance(group, DisplayGroup))
        with self.lock:
            self.display_groups.remove(group)
            self.increment()

    def find_display(self, disp_id):
        logger.debug("Finding display %d", disp_id)
        with self.lock:
            for group in self.display_groups:
                for display in group.displays:
                    if display.disp_id == disp_id:
                        return display
            return None

    def to_dict(self):
        disp_groups = []
        for group in self.display_groups:
            disp_groups.append(group.to_dict())
        return {'tid': self.tid, 'display_groups': disp_groups}


@route('/hello/<name>')
def index(name):
    return template('<b>Hello {{name}}</b>!', name=name)

@route('/files/<path:path>')
def static(path):
    return static_file(path, "../files")

@route('/app')
def main_application():
    return static_file("test_encoder.html", "../files")

@route('/topology')
def topology():
    topo = Topology.GetInstance()
    return json.dumps(topo.to_dict())

@post('/enable_display/<disp_id:int>/<enable:int>')
def enable_display(disp_id, enable):
    disp = Topology.GetInstance().find_display(disp_id)
    if disp is None:
        abort(404, "No such display (%d)" % disp_id)
    enabled = True
    if enable == 0:
        enabled = False
    try:
        disp.modify(enabled=enabled)
    except Exception as e:
        logger.error("Cannot modify display! (%s)", str(e))
        abort(500, "No such display (%d)" % disp_id)
    return '<b>ok</b>'

@post('/set_display_geometry/<disp_id:int>/<width:int>/<height:int>')
def set_display_geometry(disp_id, width, height):
    disp = Topology.GetInstance().find_display(disp_id)
    if disp is None:
        abort(404, "No such display (%d)" % disp_id)
    try:
        disp.modify(width=width, height=height)
    except Exception as e:
        logger.error("Cannot modify display! (%s)", str(e))
        abort(500, "No such display (%d)" % disp_id)
    logger.info("modify complete!")
    return '<b>ok</b>'

@post('/mouse_input/<disp_id:int>/<mouse_x:int>/<mouse_y:int>')
def mouse_input(disp_id, mouse_x, mouse_y):
    #logger.debug("Display(%d) (%d,%d)", disp_id, mouse_x, mouse_y)
    return ''

@route('/update')
def update():
    files = [f for f in os.listdir('.') if os.path.isfile(f) and f.endswith(".jpg")]
    img_file = random.choice(files)
    with open(img_file, 'rb') as f:
        def wsgi_app(environ, start_response):
            start_response('200 OK', [('Content-type', 'image/jpeg')])
            return f.read()
        return make_response(wsgi_app)

def test1():
    topo = Topology.GetInstance()
    group0 = DisplayGroup.CreateGroup(topo)
    topo.add_group(group0)
    disp0 = Display.CreateDisplay(topo, "Display0", 1400, 800, 32, True)
    disp1 = Display.CreateDisplay(topo, "Display1", 1600, 900, 32, False)
    group0.add_display(disp0)
    group0.add_display(disp1)

test1()
run(host='localhost', port=8080)
