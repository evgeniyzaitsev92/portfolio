import requests
import sys
import matplotlib.pyplot as plt
import numpy as np
import datetime
from dateutil.relativedelta import relativedelta
import time

##################
### Enter data ###
# i_ArgCount = len(sys.argv)

str_Instructions = """
*** Instructions ***

The number of arguments is 3, if periodicity is not '1-sp', otherwise is 4:
1 - Starting date, 'YYYY-MM-DD'
2 - Ending date, 'YYYY-MM-DD'
3 - Periodicity, '1-y_2-m_3-w_4-d'
4 - Starting sprint

Periodicity string can be constructed using following so-called 'calendar parts':
    * 'y' - year
    * 'q' - quarter
    * 'tr' - trimester
    * 'm' - month
    * 'w' - week
    * 'd' - day
    * 'sp' - sprint
    
and indicating the quantity of each part, for example, '2_m' stands for 2 months. 'Calendar parts' can be combined, and if so, must be separated by '_'.
Example for a <<1 year, 2 months, 3 weeks and 4 days>> periodicity: '1-y_2-m_3-w_4-d'.

The 4th arguments stand for the number of a starting sprint of a main period and is crucial if you use '1-sp' as periodicity, no other type should be combined with it: '1-sp'

The following modules should be installed via 'pip install' if necessary: matplotlib, requests, numpy, sys, datetime, time, dateutil.

*** End ***"""

i_ArgCount = len(sys.argv)
str_Period = ''
str_DateStart = ''
str_DateEnd = ''
i_Sprint = 0

if sys.argv[1] == 'instr':
    print(str_Instructions)
    sys.exit()
else:
    if (i_ArgCount != 5 and sys.argv[3] == '1-sp') or (i_ArgCount != 4 and sys.argv[3] != '1-sp'):
        print(str_Instructions)
        sys.exit()
        
    else: 
        str_DateStart = sys.argv[1]
        str_DateEnd = sys.argv[2]
        str_Period = sys.argv[3]
        if sys.argv[3] == '1-sp':
            i_Sprint = int(sys.argv[4])
            
#####################
### Authorization ###
## YouTrack
yt_url = '' # CADEX server api here
yt_user = input('Login: ')
yt_pass = input('Password: ')
yt_auth = (yt_user, yt_pass)
    
########################
### Get period pairs ###
list_Dates = []
td_Delta = relativedelta(microseconds=0)
str_PeriodForTitle = ''
arr_Period = str_Period.split('_')
calendarPart = ''

for part in arr_Period:
    calendarPart = part.split('-')[1]
    str_PeriodForTitle += ' '+part.split('-')[0]
    if calendarPart == 'y':
        td_Delta += relativedelta(years=int(part.split('-')[0]))
        str_PeriodForTitle += ' year'
    elif calendarPart == 'q':
        td_Delta += relativedelta(months=4*int(part.split('-')[0]))
        str_PeriodForTitle += ' quarter'
    elif calendarPart == 'tr':
        td_Delta += relativedelta(months=3*int(part.split('-')[0]))
        str_PeriodForTitle += ' trimester'
    elif calendarPart == 'm':
        td_Delta += relativedelta(months=int(part.split('-')[0]))
        str_PeriodForTitle += ' month'
    elif calendarPart == 'w':
        td_Delta += relativedelta(weeks=int(part.split('-')[0]))
        str_PeriodForTitle += ' week'
    elif calendarPart == 'd':
        td_Delta += relativedelta(days=int(part.split('-')[0]))
        str_PeriodForTitle += ' day'
    elif calendarPart == 'sp':
        td_Delta += relativedelta(weeks=6*int(part.split('-')[0]))
        str_PeriodForTitle += ' sprint'
    else:
        print('Unknown calendar period type!')
        print(str_Instructions)
        sys.exit()
        
    str_PeriodForTitle += '' if int(part.split('-')[0])==1 else 's'

dt_DateStart = datetime.datetime.strptime(str_DateStart, '%Y-%m-%d')
dt_DateEnd = datetime.datetime.strptime(str_DateEnd, '%Y-%m-%d')

list_Dates.append(dt_DateStart.strftime('%Y-%m-%d'))

def GetPeriodPairs(dateStart, dateEnd, delta, twoSprints=0):
    deltaTwoSprints = relativedelta(microseconds=0)
    if dateStart+delta < dateEnd:
        twoSprints += 1   
        list_Dates.append((dateStart+delta-relativedelta(days=1)).strftime('%Y-%m-%d'))
        if twoSprints==2 and str_Period == '1-sp':
            twoSprints = 0
            deltaTwoSprints = relativedelta(weeks=1)
        list_Dates.append((dateStart+delta+deltaTwoSprints).strftime('%Y-%m-%d'))
        GetPeriodPairs(dateStart+delta+deltaTwoSprints, dateEnd, delta, twoSprints)
        
    else:
        list_Dates.append(dateEnd.strftime('%Y-%m-%d'))
       
GetPeriodPairs(dt_DateStart, dt_DateEnd, td_Delta)

## Form dates pairs
num = 0
n = 1
for date in list_Dates:
    num += 1
    if (num%2) != 0:
        globals()[f"str_inputDateStart_S{n}"] = date
    else:
        globals()[f"str_inputDateEnd_S{n}"] = date
        n += 1
    
#############################
### YouTrack bugs request ###
str_bugsQuery = ''

############################## 
### Get number of requests ###
dict_Activities = {}
def GetReqNum(dateStart, dateEnd):
    list_Issues = []
    issues_params = {'fields': 'idReadable,description,customFields(name,value(name))',
                    'query': str_bugsQuery + 'State: -Rejected tag: ShowStopper'}
    request = requests.get(yt_url + 'issues', auth=yt_auth, params=issues_params)
    if (not request.ok):
        print(request.json(), file=sys.stderr)
        sys.exit(1)
    
    list_Issues.extend(request.json())
    
    ######################################### 
    ### Calculate average flow efficiency ###
    list_AvStoppersLife = []
    activities_params = {'fields': 'added(name),removed(name),timestamp,field(name),author(fullName)',
                        'categories': 'CustomFieldCategory,IssueCreatedCategory,IssueResolvedCategory'}

    ## Sprint dates
    # start date
    t_DateStart = time.strptime('{} 23:59:59'.format(dateStart), '%Y-%m-%d %H:%M:%S')
    i_DateStart = int(1000 * time.mktime(t_DateStart))
    
    # end date
    t_DateEnd = time.strptime('{} 23:59:59'.format(dateEnd), '%Y-%m-%d %H:%M:%S')
    i_DateEnd = int(1000 * time.mktime(t_DateEnd))
    
    for issue in list_Issues:
        b_GoesToList = False
        b_StateActivityFound = False
        
        ## Get activities
        requestActivities = requests.get(yt_url + 'issues/{}/activities'.format(issue['idReadable']), auth=yt_auth, params=activities_params)
        activities_chunk = requestActivities.json()
        
        i_WhenWasCreated = activities_chunk[0]['timestamp'] # date of creation
        if i_WhenWasCreated > i_DateEnd:
            continue
        
        ## Go through each list of activities
        for item in reversed(activities_chunk):
            if item['field']['name'] == 'State':
                b_StateActivityFound = True
                # Normal Close state
                if item['added'][0]['name'] == 'Closed' and item['removed'][0]['name'] != 'Closed' and item['timestamp'] > i_DateStart:
                    b_GoesToList = True
                    break
                    
                # Not closed, goes to list
                elif item['added'][0]['name'] != 'Closed':
                    b_GoesToList = True
                    break
                
        if b_StateActivityFound == False:
            b_GoesToList = True
            
        if b_GoesToList == True:
            # Date of creation
            dt_Create = datetime.datetime.fromtimestamp(i_WhenWasCreated / 1e3)
            d_CreatedDate = datetime.date(dt_Create.year, dt_Create.month, dt_Create.day)

            ## End date - closed or end of period
            i_End = i_DateEnd
            # Is closed before period end
            if item['timestamp'] > i_DateStart and item['timestamp'] < i_DateEnd:
                i_End = item['timestamp']
            
            dt_DateEnd = datetime.datetime.fromtimestamp(i_End / 1e3)
            d_DateEnd = datetime.date(dt_DateEnd.year, dt_DateEnd.month, dt_DateEnd.day)

            i_StopperLife = np.busday_count(d_CreatedDate, d_DateEnd) # consider only working days
            
            # If d_CreatedDate == d_DateEnd, np.busday_count = 0, we can consider it one working day
            if i_StopperLife == 0:
                i_StopperLife = 1

            list_AvStoppersLife.append(i_StopperLife) # add one Stopper life
            
    if len(list_AvStoppersLife) > 0:
        f64_AvStoppersLife = round((sum(list_AvStoppersLife) / len(list_AvStoppersLife)), 1)
    else:
        f64_AvStoppersLife = np.nan            

    return f64_AvStoppersLife

##################
### BUGS COUNT ###
## Get lists of TOTAL and CLOSED bugs per sprints
i_Period = 0
list_Total = [];

while i_Period != len(list_Dates)/2:
    i_Period += 1
    list_Total.append(GetReqNum(globals()[f"str_inputDateStart_S{i_Period}"], globals()[f"str_inputDateEnd_S{i_Period}"]))

################
### Graphics ###
## Lines loop
fig = plt.figure(figsize=(1.875*int(len(list_Dates)/2), 8))
   
## Misc
plt.grid(axis='y') # set grid
plt.locator_params(axis='y', integer=True) # only integers on Y axes

## Set axis bin names
i_Period = 0
i_XTicksFont = 14
X_P = []
if str_Period != '1-sp':
    i_XTicksFont = 12
    X_P = [globals()[f"str_inputDateStart_S{i+1}"]+" -\n"+globals()[f"str_inputDateEnd_S{i+1}"] for i in range(int(len(list_Dates)/2))]
else:
    while i_Period != len(list_Dates)/2:
        i_Period += 1
        if i_Sprint==9:
            i_Sprint = 1
        X_P.append(globals()[f"str_inputDateEnd_S{i_Period}"][0:4]+'_S'+str(i_Sprint)) #= [globals()[f"str_inputDateStart_S{i+1}"][0:4]+'_S'+str(list_SpintsNums[i]) for i in range(list_Dates)/2)]
        i_Sprint += 1

## Set limits
plt.xlim(-0.25, len(list_Dates)/2-0.625)

## Format ticks
plt.tick_params(direction='inout', length=8, width=1)
plt.xticks(fontsize=i_XTicksFont)
plt.yticks(fontsize=14)

## Labels locations
x = np.arange(len(X_P)) 
plt.gca().set_xticks(x, X_P) # set tasks names

## Draw lines    
plt.plot(X_P, list_Total, color='#228B22', marker='o', markersize=10, clip_on=False)

## Points labels
for x,y in zip(X_P, list_Total):
    label = str(y) # + '%' # + '%\nTotal: ' + str(globals()[f"list_TotalP{p}"][z])
    plt.annotate(label, # this is the text
                (x,y), # these are the coordinates to position the label
                textcoords="offset points", # how to position the text
                xytext=(0,10), # distance from text to points (x,y)
                ha='center', fontsize=12, fontweight='bold') # horizontal alignment can be left, right or center

## Titles
plt.xlabel('Periodicity -'+str_PeriodForTitle, labelpad=10, fontsize=20, fontweight='bold', style='italic') 
plt.ylabel('Working days', fontsize=20, fontweight='bold', style='italic')
fig.suptitle('Average Stopper Bugs Lives', fontsize=26, fontweight='bold')

## Saving the plot as an image
fig.tight_layout()
# fig.text(0, 0, "This is text on the side of the figure")
fig.subplots_adjust(wspace=0.175)
fig.savefig('Showstopper bugs.png', bbox_inches='tight', dpi=150)

print('--- *** Successfully *** ---')