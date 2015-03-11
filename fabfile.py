# -*- coding: utf-8 -*-
#  @author: Peter Morgan <pedromorgan@gmail.com>

import os

from fabric.api import env, local, run, cd, lcd, sudo, warn_only


env.hosts = [ 'fgx.freeflightsim.org' ]
env.use_ssh_config = True

PROJECT_ROOT = os.path.dirname(os.path.realpath(__file__))

REMOTE_DIR = "/home/fg/fgx"

SCREEN_SHOT_ROWS = 4 # no of rows of screenshots on www site


def _chunks(l, n):
    """ Yield successive n-sized chunks from l. (makes rows for screenshots adn nicked from stackoverflow)
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

def _read_version():
    """Read the ./version file"""
    output = local('cat version', capture=True)
    return output.strip()
    
def _get_shot_files():
    """read the files in fgx=screenshots and return jpeg atmo - todo gif etc"""
    output = local('ls fgx-screenshots', capture=True)   
    raw_files = output.split()
    
    files = []
    for file in raw_files:
        if file.endswith(".jpeg") or file.endswith(".jpg") or file.endswith(".gif") or file.endswith(".png"):
            files.append(file)
    return sorted(files)
    
def _make_screenshots_html(shots):
    """Generates the <table> html and js for the gallery"""
    s = '<div id="fgx-shots"><table>\n'
    for row in _chunks(shots, SCREEN_SHOT_ROWS):
        s += "<tr>\n"
        for i, c in enumerate(row):
            print i, c
            s += '\t<td align="center">\n'
            s += '\t\t<a href="fgx-screenshots/%s">\n' % ( c)
            s += '\t\t\t<img src="fgx-screenshots/%s" class="fgx-shots" height="120" title="%s"/></td>\n' % ( c, c)
            s += '\t\t</a>\n'
            s += '\t</td>\n'
        s += "</tr>\n"
    s += "</table></div>\n"
    
    s += "<script>\n"
    s +=  "$('#fgx-shots').magnificPopup({type:'image', delegate: 'a', gallery:{enabled: true}});\n"
    s += "</script>\n"
    return s

def _create_shots_html():
    """Created the docs/screenshots.html file which is \htmlinclude by doxygen"""
    shots =  _get_shot_files()
    html = _make_screenshots_html(shots)
    f = open("docs/screenshots.html" , "w")
    f.write(html)
    f.close()




def make_docs():
    """Generate API docs"""
    # doxygen input is not the doxygen file but overiding version on std in
    # see http://stackoverflow.com/questions/11032280/specify-doxygen-parameters-through-command-line
    
    _create_shots_html()
    ver = _read_version()
    run("git submodule init")
    run("git submodule update")
    with cd(PROJECT_ROOT + "/fgx-screenshots"):
        run("git pull origin master")
    local("cp %s/LICENSE.txt %s/docs/LICENSE.txt" % (PROJECT_ROOT, PROJECT_ROOT) ) ## doxygen is pain in butt
    local('(cat %s/docs/doxygen.fgx.conf; echo "PROJECT_NUMBER = %s") | doxygen -' % (PROJECT_ROOT, ver))
    if not os.path.exists("%s/docs_build/html/fgx-screenshots/" % (PROJECT_ROOT) ):
        local("mkdir  %s/docs_build/html/fgx-screenshots/" % (PROJECT_ROOT) )
    local("cp %s/fgx-screenshots/*.jpeg %s/docs_build/html/fgx-screenshots/" % (PROJECT_ROOT, PROJECT_ROOT) )
    local("cp %s/src/resources/artwork/fgx-logo.png %s/docs_build/html/" % (PROJECT_ROOT, PROJECT_ROOT) )
    local("cp %s/src/resources/fgx.ico %s/docs_build/html/favicon.ico" % (PROJECT_ROOT, PROJECT_ROOT) )
    local("cp %s/src/resources/fgx_48x48x32.png %s/docs_build/html/favicon.png" % (PROJECT_ROOT, PROJECT_ROOT) )
    
   
def update_www():
    """Updates www site"""
    
    local("git push origin next")
    with cd(REMOTE_DIR):
        run("git pull origin next")
        run("git submodule init")
        run("git submodule update")
        with cd(REMOTE_DIR + "/fgx-screenshots"):
            run("git pull origin master")
        run("fab make_docs")
 
   
def ssh_test():
    """Test sssh connection to fgx site"""
    run("whoami")
    run("pwd")
    run("ls -alh")
    

    
    