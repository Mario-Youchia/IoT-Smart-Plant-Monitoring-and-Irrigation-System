# -*- coding: utf-8 -*-

import itertools
import pandas as pd

WaterLevel = {"0-599", "600-1024"}
C1 = "0-599" #<600
Firebase_ready = {"0", "1"}
signupok = {"0", "1"}
getString = {"0", "1"}
SoilMoistureSensor = {"0-749", "750-1024"}
C2 = "0-749" #<750
TemperatureSensor = {"0-29", "30-50"}
C3 = "0-29" #<30


# define the ranges for each variable
ranges = [WaterLevel, Firebase_ready, signupok, getString, SoilMoistureSensor, TemperatureSensor]

# generate all possible combinations
combinations = list(itertools.product(*ranges))

# add MasterControlSignal variable to combinations
combinations = [{ "WaterLevel":combo[0], "Firebase_ready":combo[1], "signupok":combo[2], "getString":combo[3], "SoilMoistureSensor":combo[4], "TemperatureSensor":combo[5], "MasterControlSignal":combo[0] == C1, "Decision1":combo[4] != C2, "Decision2":combo[5] != C3, "ManualControl":combo[1] == "1" and combo[2] == "1" and combo[3] == "1", "SlaveControlSignal":combo[4] != C2 or combo[5] != C3 or (combo[1] == "1" and combo[2] == "1" and combo[3] == "1"), "PumpStatus":(combo[0] == C1) and ((combo[4] != C2 or combo[5] != C3 or (combo[1] == "1" and combo[2] == "1" and combo[3] == "1")) == True)} for combo in combinations]

# convert True/False to 1/0
for combo in combinations:
    for key in combo:
        if type(combo[key]) == bool:
            combo[key] = int(combo[key])

# print the combinations and MasterControlSignal
for i, combo in enumerate(combinations):
    print(i+1, combo)
    
# create a DataFrame from combinations
df = pd.DataFrame(combinations)

# save DataFrame to Excel file
df.to_csv("output.csv", index=False)