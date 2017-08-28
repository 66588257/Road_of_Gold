#pragma once


struct Incident
{
	JSONValue	json;	//���e

	Incident(JSONValue _json)
		: json(_json)
	{}
	void	action();	//���s
	void	addScuttle();
	String	name() const;
	String	description() const;
	String	type() const;
	double	time() const;
};
extern JSONReader incidentJson;
extern Array<Incident>	incidents;
