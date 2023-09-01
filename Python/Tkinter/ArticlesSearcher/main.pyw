from tkinter import *
from tkinter import ttk
import functions as f
import api
from tkhtmlview import HTMLLabel
from tkinter.messagebox import showinfo
 
win = Tk()
win.title("Articles Searcher")
logo = PhotoImage(file="img/app_logo_32.png")
win.iconphoto(False, logo)
width = 1000
height = 700
screen_width = win.winfo_screenwidth()
screen_height = win.winfo_screenheight()
x = (screen_width/2) - (width/2)
y = (screen_height/2) - (height/2)
win.geometry(f"{width}x{height}+{int(x)}+{int(y)}")
Grid.columnconfigure(win,1,weight=1)
Grid.rowconfigure(win,2,weight=1)

PAD = 3
page = 1
pages = 0
html_articles = ""

# Check if config file exists
f.check_config_exist()

# Fill request variables
f.Config.read(f.CONFIG_PATH)
req_search_entry = f.Config["Search"]["search_field"]
filters = f.Config["Filters"]
req_dt_from_enable = filters["dt_from_enable"]
req_dt_from_obj = {"year": filters["dt_from_y"], "month": filters["dt_from_m"], "day": filters["dt_from_d"]}
req_dt_to_enable = filters["dt_to_enable"]
req_dt_to_obj = {"year": filters["dt_to_y"], "month": filters["dt_to_m"], "day": filters["dt_to_d"]}
req_src_obj = {"guardian": int(filters["src_guardian"]), "nyt": int(filters["src_nyt"])}
req_num_sort_order = filters["sort_order"]

######################
### *** SEARCH *** ###
def search(cur_page):
    '''
    global req_dt_from_enable
    global req_dt_from_obj
    global req_dt_to_enable
    global req_dt_to_obj
    global req_src_obj
    global req_num_sort_order
    '''  
    global html_articles
    global html
    global pages
    global page
    
    page = cur_page
    
    f.save_config("Search", "search_field", search_entry.get())
    request = api.get_request(search_entry.get(), req_dt_from_enable.get(), req_dt_from_obj, req_dt_to_enable.get(), req_dt_to_obj, req_src_obj, req_num_sort_order.get(), page)
    # print(request)
    articles_lblframe.configure(text=f"Articles: {request['total']} results")
    pages = request["pages"]
    
    if page == 1:
        ch_page_prev_btn.configure(state="disabled")
    else:
        ch_page_prev_btn.configure(state="normal")
        
    if page == pages:
        ch_page_next_btn.configure(state="disabled")
    else:
        ch_page_next_btn.configure(state="normal")
        
    ch_page_entry.configure(state="normal")
    ch_page_entry.delete(0, END)
    ch_page_entry.insert(0, str(page))
    
    ch_page_lbl.config(text=str(pages))
    
    html_articles = '<ul style="background-color: white;">'
    
    if request["articles"] != []:
        for article in request["articles"]:
            html_articles += f'''
                <li>
                    <img src="{article['img']}" alt="" height="40" width= "40"">{article['source']} - {article['date']} - 
                    <a href="{article['url']}">{article['title']}</a>
                </li>
            '''
    else:
        html_articles += "<h1>Ничего не найдено!</h1>"
        
    html_articles += '</ul>'    
    html.destroy()
    html = HTMLLabel(articles_lblframe, html=html_articles)
    html.grid(row = 0, column = 0, padx=PAD, pady=PAD, sticky="nswe")
    
search_btn = Button(win, text="Search", command=lambda:search(1)).grid(row = 0, column = 0, padx=PAD, pady=PAD)
search_entry = Entry(win, textvariable=req_search_entry)
search_entry.bind("<Return>", lambda event:search(1))
search_entry.insert(0, req_search_entry)
search_entry.grid(row = 0, column = 1, padx=PAD, pady=PAD, sticky="we")

### *** SEARCH END *** ###
##########################

#####################
### *** ABOUT *** ###
def about():
    about_text = '''
    ************************
    *** Articles Searcher ***
    ************************
    The programme is designed to make it easier to find articles in such websites as "The Guardian" and "The New York Times".
    
    Features:
    - Key words should be separated by spaces in the search bar.
    - The whole phrase must be enclosed in double quotation marks.
    - The maximum number of results per each source is 10.
    '''
    showinfo(title="About", message=about_text)
    
about_btn = Button(win, text="?", command=lambda:about()).grid(row = 0, column = 2, padx=PAD, pady=PAD)


### *** ABOUT END *** ###
#########################

#######################
### *** FILTERS *** ###
search_lblframe = LabelFrame(win, text="Filters", font=('', 18, 'bold'))

## ** Date From Section ** ##
req_dt_from_enable = IntVar(value=req_dt_from_enable)
dt_from_checkbtn = Checkbutton(search_lblframe, text="Date from", font=('', 12, 'bold'),
                              variable=req_dt_from_enable, command=lambda:f.set_dt_fiels_state(req_dt_from_enable, #req_dt_from_enable,
                                                                                          dt_from_year_entry, dt_from_month_entry, dt_from_day_entry, 
                                                                                          "dt_from_enable")).grid(row = 0, column = 0, 
                                                                                               columnspan=6, padx=0, pady=0)
# Year
dt_from_year = StringVar()
dt_from_year.trace("w", lambda name, index, mode, var=dt_from_year: f.check_dt_entries(dt_from_year_entry, req_dt_from_obj, "year", dt_from_year, 4, "dt_from_y"))
Label(search_lblframe, text="YYYY:").grid(row = 1, column = 0)
dt_from_year_entry = Entry(search_lblframe, justify=RIGHT, width=5, textvariable=dt_from_year)
dt_from_year_entry.insert(0, req_dt_from_obj["year"])
dt_from_year_entry.grid(row = 1, column = 1, padx=PAD, pady=PAD)
dt_from_year_entry.configure(state="disabled")

# Month
dt_from_month = StringVar()
dt_from_month.trace("w", lambda name, index, mode, var=dt_from_month: f.check_dt_entries(dt_from_month_entry, req_dt_from_obj, "month", dt_from_month, 2, "dt_from_m"))
Label(search_lblframe, text="MM:").grid(row = 1, column = 2)
dt_from_month_entry = Entry(search_lblframe, justify=RIGHT, width=3, textvariable=dt_from_month)
dt_from_month_entry.insert(0, req_dt_from_obj["month"])
dt_from_month_entry.grid(row = 1, column = 3, padx=PAD, pady=PAD)
dt_from_month_entry.configure(state="disabled")

# Day
dt_from_day = StringVar()
dt_from_day.trace("w", lambda name, index, mode, var=dt_from_day: f.check_dt_entries(dt_from_day_entry, req_dt_from_obj, "day", dt_from_day, 2, "dt_from_d"))
Label(search_lblframe, text="DD:").grid(row = 1, column = 4)
dt_from_day_entry = Entry(search_lblframe, justify=RIGHT, width=3, textvariable=dt_from_day)
dt_from_day_entry.insert(0, req_dt_from_obj["day"])
dt_from_day_entry.grid(row = 1, column = 5, padx=PAD, pady=PAD)
dt_from_day_entry.configure(state="disabled")

f.set_dt_fiels_state(req_dt_from_enable, dt_from_year_entry, dt_from_month_entry, dt_from_day_entry, "dt_from_enable")

## ** Date From Section End ** ##
ttk.Separator(search_lblframe, orient='vertical').grid(row = 0, column = 6, padx=PAD, pady=PAD, rowspan=2, sticky="ns")

## ** Date To Section ** ##
req_dt_to_enable = IntVar(value=req_dt_to_enable)
dt_to_checkbtn = Checkbutton(search_lblframe, text="Date to", font=('', 12, 'bold'),
                              variable=req_dt_to_enable, command=lambda:f.set_dt_fiels_state(req_dt_to_enable, #req_dt_to_enable,
                                                                                          dt_to_year_entry, dt_to_month_entry, dt_to_day_entry, 
                                                                                          "dt_to_enable")).grid(row = 0, column = 7, 
                                                                                               columnspan=6, padx=0, pady=0)
# Year
dt_to_year = StringVar()
dt_to_year.trace("w", lambda name, index, mode, var=dt_to_year: f.check_dt_entries(dt_to_year_entry, req_dt_to_obj, "year", dt_to_year, 4, "dt_to_y"))
Label(search_lblframe, text="YYYY:").grid(row = 1, column = 7)
dt_to_year_entry = Entry(search_lblframe, justify=RIGHT, width=5, textvariable=dt_to_year)
dt_to_year_entry.insert(0, req_dt_to_obj["year"])
dt_to_year_entry.grid(row = 1, column = 8, padx=PAD, pady=PAD)
dt_to_year_entry.configure(state="disabled")

# Month
dt_to_month = StringVar()
dt_to_month.trace("w", lambda name, index, mode, var=dt_to_month: f.check_dt_entries(dt_to_month_entry, req_dt_to_obj, "month", dt_to_month, 2, "dt_to_m"))
Label(search_lblframe, text="MM:").grid(row = 1, column = 9)
dt_to_month_entry = Entry(search_lblframe, justify=RIGHT, width=3, textvariable=dt_to_month)
dt_to_month_entry.insert(0, req_dt_to_obj["month"])
dt_to_month_entry.grid(row = 1, column = 10, padx=PAD, pady=PAD)
dt_to_month_entry.configure(state="disabled")

# Day
dt_to_day = StringVar()
dt_to_day.trace("w", lambda name, index, mode, var=dt_to_day: f.check_dt_entries(dt_to_day_entry, req_dt_to_obj, "day", dt_to_day, 2, "dt_to_d"))
Label(search_lblframe, text="DD:").grid(row = 1, column = 11)
dt_to_day_entry = Entry(search_lblframe, justify=RIGHT, width=3, textvariable=dt_to_day)
dt_to_day_entry.insert(0, req_dt_to_obj["day"])
dt_to_day_entry.grid(row = 1, column = 12, padx=PAD, pady=PAD)
dt_to_day_entry.configure(state="disabled")

f.set_dt_fiels_state(req_dt_to_enable, dt_to_year_entry, dt_to_month_entry, dt_to_day_entry, "dt_to_enable")

## ** Date To Section End ** ##
ttk.Separator(search_lblframe, orient='vertical').grid(row = 0, column = 13, padx=PAD, pady=PAD, rowspan=2, sticky="ns")

## ** Source Section ** ##
Label(search_lblframe, text="Source:", font=('', 12, 'bold')).grid(row = 0, column = 14, columnspan=2)
src_guardian_enabled = IntVar(value=req_src_obj["guardian"])
src_nyt_enabled = IntVar(value=req_src_obj["nyt"])    
src_guardian_checkbtn = Checkbutton(search_lblframe, text="The Guardian",
                                    variable=src_guardian_enabled, command=lambda:f.choose_src(req_src_obj, "guardian", src_guardian_enabled, "src_guardian")).grid(row = 1, column = 14, padx=0, pady=0)
src_nyt_checkbtn = Checkbutton(search_lblframe, text="The New York Times",
                               variable=src_nyt_enabled, command=lambda:f.choose_src(req_src_obj, "nyt", src_nyt_enabled, "src_nyt")).grid(row = 1, column = 15, padx=0, pady=0)
## ** Source Section End ** ##
ttk.Separator(search_lblframe, orient='vertical').grid(row = 0, column = 16, padx=PAD, pady=PAD, rowspan=2, sticky="ns")

## ** Sort Order Section ** ##

Label(search_lblframe, text="Sort order:", font=('', 12, 'bold')).grid(row = 0, column = 17, columnspan=2)
req_num_sort_order = StringVar(value=req_num_sort_order)
sort_newest_radiobtn = Radiobutton(search_lblframe, text="Newest", value="newest", 
                                   variable=req_num_sort_order, command=lambda:f.change_sort_order(req_num_sort_order)).grid(row = 1, column = 17, padx=0, pady=0)
sort_oldest_radiobtn = Radiobutton(search_lblframe, text="Oldest", value="oldest", 
                                   variable=req_num_sort_order, command=lambda:f.change_sort_order(req_num_sort_order)).grid(row = 1, column = 18, padx=0, pady=0)

## ** Sort Order Section End ** ##

search_lblframe.grid(row = 1, column = 0, padx=PAD, pady=PAD, columnspan=3, sticky="w")
### *** FILTERS END *** ###
###########################

########################
### *** ARTICLES *** ###
articles_lblframe = LabelFrame(win, text="Articles", font=('', 18, 'bold')) 

# html = HTMLLabel(articles_lblframe, html='<h1 style="background-color: white; text-align: center"> Hello World </H1>')
html = HTMLLabel(articles_lblframe, html=html_articles)
html.grid(row = 0, column = 0, padx=PAD, pady=PAD, sticky="nswe")
# html.fit_height()

articles_lblframe.rowconfigure(0,weight=1)
articles_lblframe.columnconfigure(0,weight=1)
articles_lblframe.grid(row = 2, column = 0, padx=PAD, pady=PAD, columnspan=3, sticky="nswe")

### *** ARTICLES END *** ###
############################

###########################
### *** CHANGE PAGE *** ###
def change_page(direction):
    global page
    # global pages
    if direction == "prev":
        page -= 1
    elif direction == "next":
        page += 1
        
    search(page)
    
def change_page_from_entry(*args):
    global page
    global pages
    
    if int(ch_page_entry.get()) < 1:
        page = 1
    elif int(ch_page_entry.get()) > pages:
        page = pages
    else:
        page = int(ch_page_entry.get())
        
    search(page)
    
ch_page_frame = Frame(win)

ch_page_prev_btn = Button(ch_page_frame, text="<<<", command=lambda:change_page("prev"))
ch_page_prev_btn.configure(state="disabled")
ch_page_prev_btn.grid(row = 0, column = 0, padx=PAD, pady=PAD)

ch_page = StringVar()
ch_page.trace("w", lambda name, index, mode, var=dt_from_day: f.validate_num(ch_page_entry, ch_page, 6))
ch_page_entry = Entry(ch_page_frame, justify=RIGHT, width=7, textvariable=ch_page)
ch_page_entry.bind("<Return>", change_page_from_entry)
ch_page_entry.grid(row = 0, column = 1, padx=PAD, pady=PAD)
ch_page_entry.configure(state="disabled")

ch_page_lbl = Label(ch_page_frame, text="of 0")
ch_page_lbl.grid(row = 0, column = 2, padx=PAD, pady=PAD)

ch_page_next_btn = Button(ch_page_frame, text=">>>", command=lambda:change_page("next"))
ch_page_next_btn.grid(row = 0, column = 3, padx=PAD, pady=PAD)
ch_page_next_btn.configure(state="disabled")

ch_page_frame.grid(row = 3, column = 0, padx=PAD, pady=PAD, columnspan=3)
### *** CHANGE PAGE END *** ###
###############################

win.mainloop()