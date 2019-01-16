#pragma once

#include <string>

#include <fbxsdk/fbxsdk.h>

class Importer
{
public:
	virtual void load(const std::string & path) const = 0;

protected:
};

class FBXImporter : public Importer
{
public:
	void load(const std::string & path) const override;

private:
	void import(FbxNode * node) const;
	void import(FbxNodeAttribute * attribute) const;
	void import_mesh(FbxNodeAttribute * attribute) const;

#ifdef _DEBUG
	static void print_scene(FbxScene * scene);
	static void print_node(FbxNode * node, int tab_count);
	static void print_attribute(FbxNodeAttribute * attribute, int tab_count);
	static std::string attribute_type(FbxNodeAttribute::EType type);
	static void print_tabs(int count);
#endif
};
