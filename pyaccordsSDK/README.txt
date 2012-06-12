HOW TO INSTALL AND USE PyACCORDS SDK
       
       
By Hamid Medjahed & Elyes Zekri
       
Prologue
       
       
       
#Install pycompmanager
       
       
cd /accords-platform/pyaccordsSDK/pycompmanager
      
python setup.py install
      
      
 
      
#Generate an OCCI category
      
      
python
      
>>>import pycompmanager
      
>>>pycompmanager.generateCategory (“myCategory”, “attrbute1 attribute2”, "action1 action2",“/home/accords-platform/”)
      
>>>pycompmanager.commit(/home/accords/platform/")
      
      
      
#Remove an existing OCCI category (where '1' indicates that the category contains at least one action)
      
      
>>>pycompmanager.removeCategory("myCategory", 1,"/home/accords-platform")
      
      
      
#Generate an ACCORDS component/OCCI Server (where '2' indicates the category action number)
      
      
>>>pycompmanager.generateComponent(“myComponent”, “myCategory”, "2", “/home/accords-platform/”)
      
            
  
#Launching the component
      
      
python mycomponent.py --config config_file.xml CO-MYCOMPO-AGENT/1.0
