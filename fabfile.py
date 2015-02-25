# -*- coding: utf-8 -*-
#  @author: Peter Morgan <pedromorgan@gmail.com>

import os

from fabric.api import env, local, run, cd, lcd, sudo, warn_only

PROJECT_ROOT = os.path.dirname(os.path.realpath(__file__))

REMOTE_DIR = "/home/fg/fgx"
env.hosts = [ 'fgx.freeflightsim.org' ]
env.use_ssh_config = True


def _read_version():
    """reads the ./version_file"""
    f = open(PROJECT_ROOT + "/version", "r")
    v = f.read()
    f.close()
    return v


def make_docs():
    """Generate API docs"""
    # overiding version on std in
    # see http://stackoverflow.com/questions/11032280/specify-doxygen-parameters-through-command-line
    ver = _read_version()
    local('(cat %s/docs/doxygen.fgx.conf; echo "PROJECT_NUMBER = %s") | doxygen -' % (PROJECT_ROOT, ver))
    if not os.path.exists("%s/docs_build/html/screenshots/" % (PROJECT_ROOT) ):
        local("mkdir  %s/docs_build/html/screenshots/" % (PROJECT_ROOT) )
    local("cp %s/screenshots/*.jpeg %s/docs_build/html/screenshots/" % (PROJECT_ROOT, PROJECT_ROOT) )
    local("cp %s/src/resources/artwork/fgx-logo.png %s/docs_build/html/" % (PROJECT_ROOT, PROJECT_ROOT) )
    local("cp %s/src/resources/fgx.ico %s/docs_build/html/favicon.ico" % (PROJECT_ROOT, PROJECT_ROOT) )
   

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
        
    
    