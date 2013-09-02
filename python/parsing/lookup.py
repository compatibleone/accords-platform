from utils import Provisioner

provisioner = Provisioner()

url = provisioner.locate('price', 'onapp_configuration')
print url