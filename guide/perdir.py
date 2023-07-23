#!/usr/bin/python33
#
# Per-directory values for /lurk/guide
#
def get_type():
	return 'guide'

def get_episode_names_file():
	return '../internal/epnames'

def get_page_name_prefix():
	return 'Guide Page'

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
	return 1
