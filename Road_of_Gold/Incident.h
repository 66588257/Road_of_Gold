#pragma once


struct Incident
{
	JSONValue	json;	//���e

	Incident(JSONValue _json)
		: json(_json)
	{}
	void	action();	//���s
	void	addScuttle();
	void	update();
};
extern JSONReader incidentJson;
extern Array<Incident>	incidents;
extern Array<String>	flagTexts;