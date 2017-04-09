

void GeomDataInit(IDirect3DDevice9* device, const char* stdpath)
{
	StaticGeom::DXDevice = device;
	sprintf(StaticGeom::StdPath, "%s", stdpath);

	Green::DXDevice = device;
	sprintf(Green::StdPath, "%s", stdpath);
}