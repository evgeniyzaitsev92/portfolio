import requests
import math
from datetime import datetime
from tkinter.messagebox import showinfo

API_KEY_GUARDIAN = ""
API_KEY_NYT = ""

def get_guardian(query, dt_from_enable, dt_from_obj, dt_to_enable, dt_to_obj, sort_order, page):
    link_guardian = "https://content.guardianapis.com/search?"
    link_guardian += f"q={query}"
    link_guardian += "&format=json"
    if dt_from_enable == 1:
        link_guardian += f"&from-date={dt_from_obj['year']}-{dt_from_obj['month']}-{dt_from_obj['day']}"
    if dt_to_enable == 1:
        link_guardian += f"&to-date={dt_to_obj['year']}-{dt_to_obj['month']}-{dt_to_obj['day']}"
    link_guardian += f"&page={page}"
    link_guardian += "&page-size=10"
    link_guardian += "&show-fields=headline,thumbnail"
    link_guardian += f"&order-by={sort_order}"
    link_guardian += f"&api-key={API_KEY_GUARDIAN}"
        
    request = requests.get(link_guardian)
    return request.json()
        
def get_nyt(query, dt_from_enable, dt_from_obj, dt_to_enable, dt_to_obj, sort_order, page):
    link_nyt = "https://api.nytimes.com/svc/search/v2/articlesearch.json?"
    link_nyt += f"q={query}"
    # link_nyt += "&facet_fields=source&facet=true"
    link_nyt += f"&page={page-1}"
    link_nyt += f"&sort={sort_order}"
    if dt_from_enable == 1:
        link_nyt += f"&begin_date={dt_from_obj['year']}{dt_from_obj['month']}{dt_from_obj['day']}"
    if dt_to_enable == 1:
        link_nyt += f"&end_date={dt_to_obj['year']}{dt_to_obj['month']}{dt_to_obj['day']}" 
    link_nyt += f"&api-key={API_KEY_NYT}"
    
    request = requests.get(link_nyt)
    return request.json()

def get_request(query, dt_from_enable, dt_from_obj, dt_to_enable, dt_to_obj, src_obj, sort_order, page):
    query = query.replace(" ", ",")
    is_sort_newest = True if sort_order=="newest" else False
    req_guardian = {}
    req_nyt = {}
    total_guardian = 0
    total_nyt = 0
    articles = []
    result = {"total": 0,
              "page": page,
              "pages": 0,
              "articles": []}

    if src_obj["guardian"] == 1:
        req_guardian = get_guardian(query, dt_from_enable, dt_from_obj, dt_to_enable, dt_to_obj, sort_order, page)
        if not req_guardian["response"]["status"] == "error":
            total_guardian = req_guardian["response"]["total"]
            for article in req_guardian["response"]["results"]:
                articles.append({"source": "The Guardian",
                                 "date": datetime.fromisoformat(article["webPublicationDate"][:-1]).strftime("%B %d, %Y (%H:%M:%S)"),
                                 "title": article["webTitle"],
                                 "url": article["webUrl"],
                                 "img": "img/logo_guardian.png"})
        else:
            print("guard")
            showinfo(title="Внимание!", message=req_guardian["response"]["message"])
        
    if src_obj["nyt"] == 1 and page <= 200:
        req_nyt = get_nyt(query, dt_from_enable, dt_from_obj, dt_to_enable, dt_to_obj, sort_order, page)
        if not req_nyt["status"] == "ERROR":
            total_nyt = req_nyt["response"]["meta"]["hits"]
            for article in req_nyt["response"]["docs"]:
                articles.append({"source": "The New York Times",
                                 "date": datetime.fromisoformat(article["pub_date"][:-5]).strftime("%B %d, %Y (%H:%M:%S)"),
                                 "title": article["headline"]["main"],
                                 "url": article["web_url"],
                                 "img": "img/logo_nyt.png"})
        else:
            showinfo(title="Внимание!", message=req_nyt["errors"][0])
    
    if total_nyt > 2000:
        total_nyt = 2000
    if total_guardian > 38000:
        total_guardian = 38000
        
    result["total"] = total_guardian + total_nyt
    
    total_pages = math.ceil(max(total_guardian, total_nyt)/10)
    result["pages"] = total_pages if total_pages<=3800 else 3800
    
    articles.sort(key=lambda x: x["date"], reverse=is_sort_newest)
    result["articles"] = articles
    
    return result