Hamid Medjahed & Elyes Zekri
Prologue©



#install pycompmanager
cd /accords-platform/pyaccordsSDK/pycompmanager
python setup.py install

#generate a category
python
>>>import pycompmanager
>>>pycompmanager.generateCategory (“myCategory”, “attrbute1 attribute2”, “/home/accords-platform/”)
>>>pycompmanager.commit()

#generate an ACCORDS component
python
>>>import pycompmanager
>>>pycompmanager.generateComponent(“myComponent”, “categ1 categ2”, “/home/accords-platform/”)

#launch the component
python mycomponent.py –config config_file.xml CO-MYCOMPO-AGENT/1.0
