#!/usr/bin/env python3
import os

# -- query function --------------#
def query_var(query_string):
    if (query_string):
        query_params = query_string.split('&')
        params_dict = {}
        for param in query_params:
            if "=" in param:
                name, value = param.split('=')
                params_dict[name] = value
        return params_dict
    return None
#* -- end query function  --------#



#  -- template function ----------#
def get_page(params_dict):
    if (params_dict):
        title = params_dict.get('title', 'ff')
        heading = params_dict.get('heading', 'ff')
        message = params_dict.get('message', 'ff')
    if (os.environ.get('VALID_COOKIE') == "NO"):
        with open('./public/assets/no_cookie.html', 'r') as f:
            template = f.read()
        html = template
    elif (params_dict and len(params_dict) >= 3):
        with open('public/assets/template.html', 'r') as f:
            template = f.read()
        html = template.format(title=title, heading=heading, message=message)
    else:
        with open('public/assets/post_template.html', 'r') as f:
            template = f.read()
        html = template

    return html
#* -- end template function ------#



# ---- start env ---------------#
# CONTENT_TYPE = os.environ.get('CONTENT_TYPE', 'text/plain')
query_string = os.environ.get('QUERY_STRING', '')
body_string = os.environ.get('REQUEST_BODY', '')
request_method = os.environ.get('REQUEST_METHOD')
#* --- end env -----------------#

query_data = query_var(query_string)
body_data = query_var(body_string)

if (query_data):
    print(get_page(query_data))
else:
    print(get_page(body_data))


