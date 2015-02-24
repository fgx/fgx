# -*- coding: utf-8 -*-
#  @author: Peter Morgan <pedromorgan@gmail.com>

import os

from fabric.api import env, local, run, cd, lcd, sudo, warn_only

PROJECT_ROOT = os.path.dirname(os.path.realpath(__file__))

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
    