

#ifndef	_cords_elements_h
#define	_cords_elements_h

public struct cords_compute * allocate_cords_compute();
public struct cords_compute * liberate_cords_compute(struct cords_compute * sptr);
public struct cords_network * allocate_cords_network();
public struct cords_network * liberate_cords_network(struct cords_network * sptr);
public struct cords_storage * allocate_cords_storage();
public struct cords_storage * liberate_cords_storage(struct cords_storage * sptr);
public struct cords_security * allocate_cords_security();
public struct cords_security * liberate_cords_security(struct cords_security * sptr);
public struct cords_package * allocate_cords_package();
public struct cords_package * liberate_cords_package(struct cords_package * sptr);
public struct cords_system * allocate_cords_system();
public struct cords_system * liberate_cords_system(struct cords_system * sptr);

public struct cords_provider * allocate_cords_provider();
public struct cords_provider * liberate_cords_provider(struct cords_provider * sptr);

public struct cords_profile * allocate_cords_profile();
public struct cords_profile * liberate_cords_profile(struct cords_profile * sptr);

public struct cords_contract * allocate_cords_contract();
public struct cords_contract * liberate_cords_contract(struct cords_contract * sptr);

public struct cords_service * allocate_cords_service();
public struct cords_service * liberate_cords_service(struct cords_service * sptr);

public struct cords_tarif * allocate_cords_tarif();
public struct cords_tarif * liberate_cords_tarif(struct cords_tarif * sptr);


public struct cords_image * allocate_cords_image();
public struct cords_image * liberate_cords_image(struct cords_image * sptr);
public struct cords_infrastructure * allocate_cords_infrastructure();
public struct cords_infrastructure * liberate_cords_infrastructure(struct cords_infrastructure * sptr);
public struct cords_core * allocate_cords_core();
public struct cords_core * liberate_cords_core(struct cords_core * sptr);
public struct cords_action * allocate_cords_action();
public struct cords_action * liberate_cords_action(struct cords_action * sptr);
public struct cords_user * allocate_cords_user();
public struct cords_user * liberate_cords_user(struct cords_user * sptr);
public struct cords_configuration * allocate_cords_configuration();
public struct cords_configuration * liberate_cords_configuration(struct cords_configuration * sptr);
public struct cords_constraint * allocate_cords_constraint();
public struct cords_constraint * liberate_cords_constraint(struct cords_constraint * sptr);
public struct cords_requirement * allocate_cords_requirement();
public struct cords_requirement * liberate_cords_requirement(struct cords_requirement * sptr);
public struct cords_account * allocate_cords_account();
public struct cords_account * liberate_cords_account(struct cords_account * sptr);
public struct cords_manifest * allocate_cords_manifest();
public struct cords_manifest * liberate_cords_manifest(struct cords_manifest * sptr);
public struct cords_plan * allocate_cords_plan();
public struct cords_plan * liberate_cords_plan(struct cords_plan * sptr);
public struct cords_link * allocate_cords_link();
public struct cords_link * liberate_cords_link(struct cords_link * sptr);
public struct publication * allocate_publication();
public struct publication * liberate_publication(struct publication * sptr);
public struct enquiry * allocate_enquiry();
public struct enquiry * liberate_enquiry(struct enquiry * sptr);
public struct agency * allocate_agency();
public struct agency * liberate_agency(struct agency * sptr);
public struct openstack * allocate_openstack();
public struct openstack * liberate_openstack(struct openstack * sptr);
public struct on_config * allocate_on_config();
public struct on_config * liberate_on_config(struct on_config * sptr);
public struct os_config * allocate_os_config();
public struct os_config * liberate_os_config(struct os_config * sptr);
public struct opennebula * allocate_opennebula();
public struct opennebula * liberate_opennebula(struct opennebula * sptr);

#endif	/* _cords_elements_h */
	/* ----------------- */



