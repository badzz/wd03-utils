# wd03-utils
Tools for the Ravpower wd03

Mainly this is a tool to find the battery level and charging/not charging status of the Ravpower WD03

The main SOC (MT7620N) is connected via I2C to a Holtek HT66F40 that does battery management of the 6000mah battery. 
By snooping what the stock firmware does, I manage to find the commands being used. 

Thus I replicated those commands to get the same functionality.

Thanks
