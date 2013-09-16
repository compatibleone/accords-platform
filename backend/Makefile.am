# This makefile will be parsed to create Makefile.in


lib_LTLIBRARIES = libbackend.la

# Build the model files
model_h_files = xlink.h windowsazure.h cnconfig.h computenext.h publication.h authitem.h \
    easiclouds_application.h easiclouds_connection.h easiclouds_description.h easiclouds_extra.h \
    easiclouds_instance.h easiclouds_keypair.h easiclouds_link.h easiclouds_metadata.h \
    easiclouds_model.h easiclouds_node.h easiclouds_parameters.h \
    easiclouds_postconf.h easiclouds_server.h easiclouds_input.h easiclouds_output.h \
    saas.h \
    eziconfig.h easiclouds.h \
    job.h workload.h \
    nagioscommand.h nagioscontact.h nagioscontactgroup.h nagioshost.h nagioshostdependancy.h nagioshostescalation.h \
    nagioshostextinfo.h nagioshostgroup.h nagiosservice.h nagiosserviceescalation.h nagiosservicegroup.h \
    nagiostimeperiod.h nagiosservicedependancy.h nagiosserviceextinfo.h \
    paas_application.h paas_application_deployable.h paas_application_manifest.h \
	paas_application_version.h paas_application_version_instance.h paas_environment.h \
	paas_environment_configuration.h paas_environment_link.h paas_environment_node.h \
	paas_environment_relation.h paas_environment_template.h paas_environment_variable.h \
	quantity.h federation.h \
	azconfig.h paconfig.h placement.h account.h instruction.h		\
	node.h package.h tarif.h carrier.h report.h action.h invoice.h onconfig.h 			\
	parameter.h transaction.h compute.h domain.h requirement.h agency.h ipaddress.h		\
	opennebula.h plan.h user.h configuration.h enquiry.h security.h algorithm.h			\
	price.h value.h constraint.h event.h manifest.h service.h appliance.h	\
	profile.h contract.h image.h monitor.h optimise.h storage.h			\
	authorise.h provider.h infrastructure.h network.h osconfig.h system.h	\
	iprange.h instance.h operator.h vm.h port.h firewall.h import.h agreement.h			\
	metadata.h file.h script.h interface.h session.h consumer.h connection.h stream.h	\
	probe.h metric.h alert.h quota.h application.h variable.h business.h zone.h			\
	guarantee.h term.h terms.h amazonEc2.h control.h			\
	ec2config.h gw.h intercloudGW.h linkgw.h packet.h penalty.h release.h volume.h \
	counter.h schedule.h timer.h \
	deltacloud.h dcconfig.h \
	dummy.h dummyconfig.h \
	exampleprocci.h \
	onapp.h oaconfig.h onapp_extras.h \
	openstack.h \
	paas.h paasconfig.h
	
include $(top_srcdir)/tools/Makefile_generate_model.am

libbackend_la_SOURCES = backend_common.c link_backend.c backend_profiling.c riak_backend.c \
    xlink_node_backend.c occi_rest_xlink.c xlink.c \
    cnconfig_node_backend.c occi_rest_cnconfig.c cnconfig.c \
    computenext_node_backend.c occi_rest_computenext.c computenext.c \
    windowsazure_node_backend.c occi_rest_windowsazure.c windowsazure.c \
    publication_riak_backend.c occi_rest_publication.c publication.c \
    authitem_node_backend.c occi_rest_authitem.c authitem.c \
    easiclouds_application_node_backend.c occi_rest_easiclouds_application.c easiclouds_application.c \
    easiclouds_connection_node_backend.c occi_rest_easiclouds_connection.c easiclouds_connection.c \
    easiclouds_description_node_backend.c occi_rest_easiclouds_description.c easiclouds_description.c \
    easiclouds_extra_node_backend.c occi_rest_easiclouds_extra.c easiclouds_extra.c \
    easiclouds_instance_node_backend.c occi_rest_easiclouds_instance.c easiclouds_instance.c \
    easiclouds_keypair_node_backend.c occi_rest_easiclouds_keypair.c easiclouds_keypair.c \
    easiclouds_link_node_backend.c occi_rest_easiclouds_link.c easiclouds_link.c \
    easiclouds_metadata_node_backend.c occi_rest_easiclouds_metadata.c easiclouds_metadata.c \
    easiclouds_model_node_backend.c occi_rest_easiclouds_model.c easiclouds_model.c \
    easiclouds_node_node_backend.c occi_rest_easiclouds_node.c easiclouds_node.c \
    easiclouds_parameters_node_backend.c occi_rest_easiclouds_parameters.c easiclouds_parameters.c \
    easiclouds_postconf_node_backend.c occi_rest_easiclouds_postconf.c easiclouds_postconf.c \
    easiclouds_server_node_backend.c occi_rest_easiclouds_server.c easiclouds_server.c \
    easiclouds_input_node_backend.c occi_rest_easiclouds_input.c easiclouds_input.c \
    easiclouds_output_node_backend.c occi_rest_easiclouds_output.c easiclouds_output.c \
    saas_node_backend.c occi_rest_saas.c saas.c \
    eziconfig_node_backend.c occi_rest_eziconfig.c eziconfig.c \
    easiclouds_node_backend.c occi_rest_easiclouds.c easiclouds.c \
    job_node_backend.c occi_rest_job.c job.c \
    workload_node_backend.c occi_rest_workload.c workload.c \
    nagioscommand_node_backend.c occi_rest_nagioscommand.c nagioscommand.c \
    nagioscontact_node_backend.c occi_rest_nagioscontact.c nagioscontact.c \
    nagioscontactgroup_node_backend.c occi_rest_nagioscontactgroup.c nagioscontactgroup.c \
    nagioshost_node_backend.c occi_rest_nagioshost.c nagioshost.c \
    nagioshostdependancy_node_backend.c occi_rest_nagioshostdependancy.c nagioshostdependancy.c \
    nagioshostescalation_node_backend.c occi_rest_nagioshostescalation.c nagioshostescalation.c \
    nagioshostextinfo_node_backend.c occi_rest_nagioshostextinfo.c nagioshostextinfo.c \
    nagioshostgroup_node_backend.c occi_rest_nagioshostgroup.c nagioshostgroup.c \
    nagiosservice_node_backend.c occi_rest_nagiosservice.c nagiosservice.c \
    nagiosserviceescalation_node_backend.c occi_rest_nagiosserviceescalation.c nagiosserviceescalation.c \
    nagiosservicegroup_node_backend.c occi_rest_nagiosservicegroup.c nagiosservicegroup.c \
    nagiostimeperiod_node_backend.c occi_rest_nagiostimeperiod.c nagiostimeperiod.c \
    nagiosservicedependancy_node_backend.c occi_rest_nagiosservicedependancy.c nagiosservicedependancy.c \
    nagiosserviceextinfo_node_backend.c occi_rest_nagiosserviceextinfo.c nagiosserviceextinfo.c \
    paas_application_node_backend.c occi_rest_paas_application.c paas_application.c \
    paas_application_deployable_node_backend.c occi_rest_paas_application_deployable.c paas_application_deployable.c \
    paas_application_manifest_node_backend.c occi_rest_paas_application_manifest.c paas_application_manifest.c \
    paas_application_version_node_backend.c occi_rest_paas_application_version.c paas_application_version.c \
    paas_application_version_instance_node_backend.c occi_rest_paas_application_version_instance.c paas_application_version_instance.c \
    paas_environment_node_backend.c occi_rest_paas_environment.c paas_environment.c \
    paas_environment_configuration_node_backend.c occi_rest_paas_environment_configuration.c paas_environment_configuration.c \
    paas_environment_link_node_backend.c occi_rest_paas_environment_link.c paas_environment_link.c \
    paas_environment_node_node_backend.c occi_rest_paas_environment_node.c paas_environment_node.c \
    paas_environment_relation_node_backend.c occi_rest_paas_environment_relation.c paas_environment_relation.c \
    paas_environment_template_node_backend.c occi_rest_paas_environment_template.c paas_environment_template.c \
    paas_environment_variable_node_backend.c occi_rest_paas_environment_variable.c paas_environment_variable.c \
    quantity_node_backend.c occi_rest_quantity.c quantity.c \
    federation_node_backend.c occi_rest_federation.c federation.c \
    azconfig_node_backend.c occi_rest_azconfig.c azconfig.c \
    paconfig_node_backend.c occi_rest_paconfig.c paconfig.c \
    placement_node_backend.c occi_rest_placement.c placement.c \
    account_node_backend.c occi_rest_account.c account.c \
    instruction_node_backend.c occi_rest_instruction.c instruction.c \
    node_node_backend.c occi_rest_node.c node.c \
    package_node_backend.c occi_rest_package.c package.c \
    tarif_node_backend.c occi_rest_tarif.c tarif.c \
    carrier_node_backend.c occi_rest_carrier.c carrier.c \
    report_node_backend.c occi_rest_report.c report.c \
    action_node_backend.c occi_rest_action.c action.c \
    invoice_node_backend.c occi_rest_invoice.c invoice.c \
    onconfig_node_backend.c occi_rest_onconfig.c onconfig.c \
    parameter_node_backend.c occi_rest_parameter.c parameter.c \
    transaction_node_backend.c occi_rest_transaction.c transaction.c \
    compute_node_backend.c occi_rest_compute.c compute.c \
    domain_node_backend.c occi_rest_domain.c domain.c \
    requirement_node_backend.c occi_rest_requirement.c requirement.c \
    agency_node_backend.c occi_rest_agency.c agency.c \
    ipaddress_node_backend.c occi_rest_ipaddress.c ipaddress.c \
    plan_node_backend.c occi_rest_plan.c plan.c \
    user_node_backend.c occi_rest_user.c user.c \
    configuration_node_backend.c occi_rest_configuration.c configuration.c \
    enquiry_node_backend.c occi_rest_enquiry.c enquiry.c \
    security_node_backend.c occi_rest_security.c security.c \
    algorithm_node_backend.c occi_rest_algorithm.c algorithm.c \
    openstack_node_backend.c occi_rest_openstack.c openstack.c \
    price_node_backend.c occi_rest_price.c price.c \
    value_node_backend.c occi_rest_value.c value.c \
    constraint_node_backend.c occi_rest_constraint.c constraint.c \
    event_node_backend.c occi_rest_event.c event.c \
    manifest_node_backend.c occi_rest_manifest.c manifest.c \
    service_node_backend.c occi_rest_service.c service.c \
    appliance_node_backend.c occi_rest_appliance.c appliance.c \
    profile_node_backend.c occi_rest_profile.c profile.c \
    contract_node_backend.c occi_rest_contract.c contract.c \
    image_node_backend.c occi_rest_image.c image.c \
    monitor_node_backend.c occi_rest_monitor.c monitor.c \
    optimise_node_backend.c occi_rest_optimise.c optimise.c \
    storage_node_backend.c occi_rest_storage.c storage.c \
    authorise_node_backend.c occi_rest_authorise.c authorise.c \
    provider_node_backend.c occi_rest_provider.c provider.c \
    infrastructure_node_backend.c occi_rest_infrastructure.c infrastructure.c \
    network_node_backend.c occi_rest_network.c network.c \
    osconfig_node_backend.c occi_rest_osconfig.c osconfig.c \
    system_node_backend.c occi_rest_system.c system.c \
    iprange_node_backend.c occi_rest_iprange.c iprange.c \
    instance_node_backend.c occi_rest_instance.c instance.c \
    operator_node_backend.c occi_rest_operator.c operator.c \
    vm_node_backend.c occi_rest_vm.c vm.c \
    port_node_backend.c occi_rest_port.c port.c \
    firewall_node_backend.c occi_rest_firewall.c firewall.c \
    import_node_backend.c occi_rest_import.c import.c \
    agreement_node_backend.c occi_rest_agreement.c agreement.c \
    metadata_node_backend.c occi_rest_metadata.c metadata.c \
    file_node_backend.c occi_rest_file.c file.c \
    script_node_backend.c occi_rest_script.c script.c \
    interface_node_backend.c occi_rest_interface.c interface.c \
    session_node_backend.c occi_rest_session.c session.c \
    consumer_node_backend.c occi_rest_consumer.c consumer.c \
    connection_node_backend.c occi_rest_connection.c connection.c \
    stream_node_backend.c occi_rest_stream.c stream.c \
    probe_node_backend.c occi_rest_probe.c probe.c \
    metric_node_backend.c occi_rest_metric.c metric.c \
    alert_node_backend.c occi_rest_alert.c alert.c \
    quota_node_backend.c occi_rest_quota.c quota.c \
    application_node_backend.c occi_rest_application.c application.c \
    variable_node_backend.c occi_rest_variable.c variable.c \
    business_node_backend.c occi_rest_business.c business.c \
    zone_node_backend.c occi_rest_zone.c zone.c \
    guarantee_node_backend.c occi_rest_guarantee.c guarantee.c \
    term_node_backend.c occi_rest_term.c term.c \
    terms_node_backend.c occi_rest_terms.c terms.c \
    amazonEc2_node_backend.c occi_rest_amazonEc2.c amazonEc2.c \
    control_node_backend.c occi_rest_control.c control.c \
    ec2config_node_backend.c occi_rest_ec2config.c ec2config.c \
    gw_node_backend.c occi_rest_gw.c gw.c \
    intercloudGW_node_backend.c occi_rest_intercloudGW.c intercloudGW.c \
    linkgw_node_backend.c occi_rest_linkgw.c linkgw.c \
    packet_node_backend.c occi_rest_packet.c packet.c \
    penalty_node_backend.c occi_rest_penalty.c penalty.c \
    release_node_backend.c occi_rest_release.c release.c \
    volume_node_backend.c occi_rest_volume.c volume.c \
    counter_node_backend.c occi_rest_counter.c counter.c \
    schedule_node_backend.c occi_rest_schedule.c schedule.c \
    timer_node_backend.c occi_rest_timer.c timer.c \
    deltacloud_node_backend.c occi_rest_deltacloud.c deltacloud.c \
    dcconfig_node_backend.c occi_rest_dcconfig.c dcconfig.c \
    dummy_node_backend.c occi_rest_dummy.c dummy.c \
    dummyconfig_node_backend.c occi_rest_dummyconfig.c dummyconfig.c \
    exampleprocci_node_backend.c occi_rest_exampleprocci.c exampleprocci.c \
    onapp_node_backend.c occi_rest_onapp.c onapp.c \
    oaconfig_node_backend.c occi_rest_oaconfig.c oaconfig.c \
    onapp_extras_node_backend.c occi_rest_onapp_extras.c onapp_extras.c \
    opennebula_node_backend.c occi_rest_opennebula.c opennebula.c \
    paas_node_backend.c occi_rest_paas.c paas.c \
    paasconfig_node_backend.c occi_rest_paasconfig.c paasconfig.c 
    
libbackend_la_CFLAGS = $(CFLAGS_BACKEND) $(CFLAGS_COXML) $(CFLAGS_OCCI) $(CFLAGS_COREST) -Werror
libbackend_la_LDFLAGS = -lcurl 

#json-c
JSON_C_DIR=/usr/local
libbackend_la_CFLAGS += -I$(JSON_C_DIR)/include/json-c
libbackend_la_LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c

link_backend.c : $(model_c_files) generator_input.txt
