#!/usr/bin/python
#
# Per-directory values for /lurk/novels
#
def get_type():
	return 'novels'

def get_episode_names_file():
	return '../internal/novelnames'

def get_page_name_prefix():
	return 'Novel'

def get_table_of_contents():
	return """

<p>
<b>Contents:</b>
<a href="#BC">Back Cover</a> -
<a href="#SY">Synopsis</a> -
<a href="#UQ">Questions</a> -
<a href="#AN">Analysis</a> -
<a href="#NO">Notes</a> -
<a href="#JS">Author</a>
 
"""

def get_do_jpeg():
	return 0
