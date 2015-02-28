# -*- coding: utf-8 -*-
#  @author: Peter Morgan <pedromorgan@gmail.com>

import os


from fabric.api import env, local, run, cd, lcd, sudo, warn_only

PROJECT_ROOT = os.path.dirname(os.path.realpath(__file__))

REMOTE_DIR = "/home/fg/fgx"

env.hosts = [ 'fgx.freeflightsim.org' ]
env.use_ssh_config = True

def _chunks(l, n):
    """ Yield successive n-sized chunks from l.
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

def _read_version(is_remote):
    if is_remote:
        output = run('cat version')
    else:
        output = local('cat version', capture=True)
    return output.strip()
    """reads the ./version_file"""
    f = open(path + "/version", "r")
    v = f.read()
    f.close()
    return v

def _get_shot_files(is_remote):
    if is_remote:
        output = run('ls fgx-screenshots')
    else:
        output = local('ls fgx-screenshots', capture=True)
        
    raw_files = output.split()
    #print "=======", raw_files
    
    files = []
    for file in raw_files:
        if file.endswith(".jpeg"):
            files.append(file)
    # print files
    #print "------------------"
    return sorted(files)
    
def _make_screenshots_html(shots):
    
    s = '<div id="fgx-shots"><table>\n'
    for row in _chunks(shots, 3):
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

def _create_shots_html(is_remote):
    shots =  _get_shot_files(is_remote)
    html = _make_screenshots_html(shots)
    f = open("docs/screenshots.html" , "w")
    f.write(html)
    f.close()

def make_docs():
    """Generate API docs"""
    # overiding version on std in
    # see http://stackoverflow.com/questions/11032280/specify-doxygen-parameters-through-command-line
    #shots =  _get_shot_files(PROJECT_ROOT)
    #html = _make_screenshots_html(shots)
    print _create_shots_html(False)
    ver = _read_version(False)
    local("cp %s/LICENSE.txt %s/docs/LICENSE.txt" % (PROJECT_ROOT, PROJECT_ROOT) ) ## doxygen is pain in butt
    local('(cat %s/docs/doxygen.fgx.conf; echo "PROJECT_NUMBER = %s") | doxygen -' % (PROJECT_ROOT, ver))
    if not os.path.exists("%s/docs_build/html/fgx-screenshots/" % (PROJECT_ROOT) ):
        local("mkdir  %s/docs_build/html/fgx-screenshots/" % (PROJECT_ROOT) )
    local("cp %s/fgx-screenshots/*.jpeg %s/docs_build/html/fgx-screenshots/" % (PROJECT_ROOT, PROJECT_ROOT) )
    local("cp %s/src/resources/artwork/fgx-logo.png %s/docs_build/html/" % (PROJECT_ROOT, PROJECT_ROOT) )
    local("cp %s/src/resources/fgx.ico %s/docs_build/html/favicon.ico" % (PROJECT_ROOT, PROJECT_ROOT) )
   
def update_www():
    """Generate REMOTE API docs"""
    # overiding version on std in
    # see http://stackoverflow.com/questions/11032280/specify-doxygen-parameters-through-command-line
    with cd(REMOTE_DIR):
        run("git pull origin next")
        run("git submodule update")
        with cd(REMOTE_DIR + "/fgx-screenshots"):
            run("git pull origin master")
        run("fab make_docs")
 
   
def ssh_test():
    """Test sssh connection to fgx site"""
    run("whoami")
    run("pwd")
    run("ls -alh")
    
          
def up_www():
    """Updates the online website"""
    with cd(REMOTE_DIR):
        run("git pull")
        run("fab make_docs")
        
    
    