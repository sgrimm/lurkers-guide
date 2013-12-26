#!/usr/bin/python
#
# Per-directory values for /lurk/comic
#
def get_type():
	return 'comic'

def get_episode_names_file():
	return '../internal/comicnames'

def get_page_name_prefix():
	return 'Comic'

def get_table_of_contents():
	return """

<p>
<b>Contents:</b>
<a href="#OV">Overview</a> -
<a href="#BP">Backplot</a> -
<a href="#UQ">Questions</a> -
<a href="#AN">Analysis</a> -
<a href="#NO">Notes</a> -
<a href="#JS">JMS</a>

"""

def get_do_jpeg():
	return 0
