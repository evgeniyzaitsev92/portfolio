from backports import configparser as cp
import os.path as path

######################
### *** COMMON *** ###
CONFIG_PATH = "config.ini"
Config = cp.ConfigParser()
def create_config():
    global CONFIG_PATH
    Config = cp.ConfigParser()
    
    Config.add_section("Search")
    Config.set("Search", "search_field", "")
    
    Config.add_section("Filters")
    Config.set("Filters", "dt_from_enable", "0")
    Config.set("Filters", "dt_from_y", "")
    Config.set("Filters", "dt_from_m", "")
    Config.set("Filters", "dt_from_d", "")
    Config.set("Filters", "dt_to_enable", "0")
    Config.set("Filters", "dt_to_y", "")
    Config.set("Filters", "dt_to_m", "")
    Config.set("Filters", "dt_to_d", "")
    Config.set("Filters", "src_guardian", "0")
    Config.set("Filters", "src_nyt", "0")
    Config.set("Filters", "sort_order", "newest")
    
    with open(CONFIG_PATH, 'w') as configfile:
        Config.write(configfile)

def check_config_exist():
    global CONFIG_PATH
    if not path.exists(CONFIG_PATH):
        create_config()
        
def save_config(section, var, value):
    global CONFIG_PATH
    check_config_exist()
    Config = cp.ConfigParser()
    Config.read(CONFIG_PATH)
    
    if not Config.has_section(section):
        Config.add_section(section)
    if not Config.has_option(section, var):
        Config.set(section, var, "")
        
    Section = Config[section]
    Section[var] = value
    with open(CONFIG_PATH, 'w') as configfile:
        Config.write(configfile)
    
#######################
### *** FILTERS *** ###
# Validate number
def validate_num(widget, value, limit):
    # check if interger
    try:
        int(value.get())
    except:
        widget.delete(len(value.get())-1)
       
    # limit num of digits
    if len(value.get()) > limit:
        widget.delete(limit)
        
# Check whether dates fields are filled with valid info
def check_dt_entries(widget, req_dt_obj, dt_part, value, limit, config_value):
    # check if valid
    validate_num(widget, value, limit)
        
    # fill date array
    if dt_part == "year":
        req_dt_obj["year"] = widget.get()
    elif dt_part == "month":
        req_dt_obj["month"] = widget.get()
    elif dt_part == "day":
        req_dt_obj["day"] = widget.get()
        
    save_config("Filters", config_value, widget.get())
    
# Enable/disable dates fields
def set_dt_fiels_state(widget_enabled, widget_year, widget_month, widget_day, config_value):
    save_config("Filters", config_value, str(widget_enabled.get()))
    if widget_enabled.get() == 0:
        widget_year.configure(state="disabled")
        widget_month.configure(state="disabled")
        widget_day.configure(state="disabled")
    else:
        widget_year.configure(state="normal")
        widget_month.configure(state="normal")
        widget_day.configure(state="normal")
        
# Choose source
def choose_src(req_src_obj, src_type, widget_enabled, config_value):
    req_src_obj[src_type] = int(widget_enabled.get())
    save_config("Filters", config_value, str(widget_enabled.get()))
    
# Change sort order
def change_sort_order(req_num_sort_order):
    save_config("Filters", "sort_order", req_num_sort_order.get())