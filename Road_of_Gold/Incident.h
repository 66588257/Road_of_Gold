#pragma once

struct Incident
{
	JSONValue	json;	//���e

	Incident(JSONValue _json);
	String	name() const;
	String	description() const;
	String	type() const;
	double	time() const;
};

extern Array<Incident>	incidents;
