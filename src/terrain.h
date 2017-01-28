// terrain.h

#ifndef _TERRAIN_H_
#define	_TERRAIN_H_


class CInstanceManager;
class CD3DEngine;
class CWater;



#define FLATLIMIT	(5.0f*PI/180.0f)
#define MAXTRAJECT	200


enum TerrainRes
{
	TR_NULL		= 0,
	TR_STONE	= 1,
	TR_URANIUM	= 2,
	TR_POWER	= 3,
	TR_KEYa		= 4,
	TR_KEYb		= 5,
	TR_KEYc		= 6,
	TR_KEYd		= 7,
};


#define MAXBUILDINGLEVEL	100

typedef struct
{
	D3DVECTOR	center;
	float		factor;
	float		min;
	float		max;
	float		level;
	float		height;
	float		bboxMinX;
	float		bboxMaxX;
	float		bboxMinZ;
	float		bboxMaxZ;
}
BuildingLevel;


#define MAXSLOWERZONE	10

typedef struct
{
	D3DVECTOR	center;
	float		min;
	float		max;
	float		factor;
	float		bboxMinX;
	float		bboxMaxX;
	float		bboxMinZ;
	float		bboxMaxZ;
}
SlowerZone;


#define MAXMATTERRAIN		100

typedef struct
{
	short		id;
	char		texName[20];
	float		u,v;
	float		hardness;
	char		mat[4];		// up, right, down, left
}
TerrainMaterial;

typedef struct
{
	short		id;
	char		mat[4];		// up, right, down, left
}
DotLevel;


#define MAXFLYINGLIMIT	10

typedef struct
{
	D3DVECTOR	center;
	float		extRadius;
	float		intRadius;
	float		maxHeight;
}
FlyingLimit;



class CTerrain
{
public:
	CTerrain(CInstanceManager* iMan);
	~CTerrain();

	BOOL		Generate(int mosaic, int brickP2, float size, float vision, int depth, float hardness);
	BOOL		InitTextures(char* baseName, int* table, int dx, int dy);
	void		LevelFlush();
	BOOL		LevelMaterial(int id, char* baseName, float u, float v, int up, int right, int down, int left, float hardness);
	BOOL		LevelInit(int id);
	BOOL		LevelGenerate(int *id, float min, float max, float slope, float freq, D3DVECTOR center, float radius);
	void		LevelRoadAdapt(BOOL bF1);
	void		FlushRelief();
	BOOL		ReliefFromBMP(const char* filename, float scaleRelief);
	BOOL		ReliefFromDXF(const char* filename, float scaleRelief);
	BOOL		CreateObjects(BOOL bMultiRes);
	BOOL		Terraform(const D3DVECTOR &p1, const D3DVECTOR &p2, float height);

	void		SetWind(D3DVECTOR speed);
	D3DVECTOR	RetWind();

	float		RetFineSlope(const D3DVECTOR &pos);
	float		RetCoarseSlope(const D3DVECTOR &pos);
	BOOL		GetNormal(D3DVECTOR &n, const D3DVECTOR &p);
	float		RetFloorLevel(const D3DVECTOR &p, BOOL bBrut=FALSE, BOOL bWater=FALSE);
	float		RetFloorHeight(const D3DVECTOR &p, BOOL bBrut=FALSE, BOOL bWater=FALSE);
	BOOL		MoveOnFloor(D3DVECTOR &p, BOOL bBrut=FALSE, BOOL bWater=FALSE);
	BOOL		ValidPosition(D3DVECTOR &p, float marging);
	void		LimitPos(D3DVECTOR &pos);

	void		FlushBuildingLevel();
	BOOL		AddBuildingLevel(D3DVECTOR center, float min, float max, float height, float factor);
	BOOL		UpdateBuildingLevel(D3DVECTOR center);
	BOOL		DeleteBuildingLevel(D3DVECTOR center);
	float		RetBuildingFactor(const D3DVECTOR &p);

	void		FlushSlowerZone();
	BOOL		AddSlowerZone(D3DVECTOR center, float min, float max, float factor);
	BOOL		DeleteSlowerZone(D3DVECTOR center);
	float		RetSlowerZone(const D3DVECTOR &p);

	float		RetHardness(const D3DVECTOR &p);

	int			RetMosaic();
	int			RetBrick();
	float		RetSize();
	float		RetScaleRelief();

	float		RetFlatZoneRadius(D3DVECTOR center, float max);

	void		SetFlyingMaxHeight(float height);
	float		RetFlyingMaxHeight();
	void		FlushFlyingLimit();
	BOOL		AddFlyingLimit(D3DVECTOR center, float extRadius, float intRadius, float maxHeight);
	float		RetFlyingLimit(D3DVECTOR pos, BOOL bNoLimit);

	void		FlushTraject();
	BOOL		AddTraject(const D3DVECTOR &pos);
	BOOL		GetTraject(int rank, D3DVECTOR &pos);

protected:
	BOOL		ReliefAddDot(D3DVECTOR pos, float scaleRelief);
	void		AdjustRelief();
	D3DVECTOR	RetVector(int x, int y);
	D3DVERTEX2	RetVertex(int x, int y, int step);
	BOOL		CreateMosaic(int ox, int oy, int step, int objRank, const D3DMATERIAL7 &mat, float min, float max);
	BOOL		CreateSquare(BOOL bMultiRes, int x, int y);

	TerrainMaterial* LevelSearchMat(int id);
	void		LevelTextureName(int x, int y, char *name, FPOINT &uv);
	float		LevelRetHeight(int x, int y);
	BOOL		LevelGetDot(int x, int y, float min, float max, float slope);
	int			LevelTestMat(char *mat);
	void		LevelSetDot(int x, int y, int id, char *mat);
	BOOL		LevelIfDot(int x, int y, int id, char *mat);
	BOOL		LevelPutDot(int x, int y, int id);
	void		LevelOpenTable();
	void		LevelCloseTable();

	int			RoadSearchID(int bits);
	int			RoadSearchBits(int id);
	int			RoadGetID(int x, int y);
	int			RoadSearchBitsFull(int x, int y, BOOL bF1);
	int			RoadSearchBitsDiag(int x, int y);

	void		AdjustBuildingLevel(D3DVECTOR &p);

protected:
	CInstanceManager* m_iMan;
	CD3DEngine*		m_engine;
	CWater*			m_water;

	int				m_mosaic;		// nb de mosa�que
	int				m_brick;		// nb de briques par mosa�que
	float			m_size;			// taille d'un �l�ment dans une brique
	float			m_vision;		// vision avant un changement de r�solution
	float*			m_relief;		// table du relief
	int*			m_texture;		// table des textures
	int*			m_objRank;		// table des rangs des objets
	BOOL			m_bMultiText;
	BOOL			m_bLevelText;
	float			m_scaleMapping;	// �chelle du mapping
	float			m_scaleRelief;
	int				m_subdivMapping;
	int				m_depth;		// nb de r�solutions diff�rentes (1,2,3,4)
	char			m_texBaseName[20];
	char			m_texBaseExt[10];
	float			m_defHardness;
	
	TerrainMaterial	m_levelMat[MAXMATTERRAIN+1];
	int				m_levelMatTotal;
	int				m_levelMatMax;
	int				m_levelDotSize;
	DotLevel*		m_levelDot;
	int				m_levelID;

	int				m_buildingUsed;
	BuildingLevel	m_buildingTable[MAXBUILDINGLEVEL];

	int				m_slowerUsed;
	BuildingLevel	m_slowerTable[MAXSLOWERZONE];

	D3DVECTOR		m_wind;			// vitesse du vent

	float			m_flyingMaxHeight;
	int				m_flyingLimitTotal;
	FlyingLimit		m_flyingLimit[MAXFLYINGLIMIT];

	int				m_trajectTotal;
	D3DVECTOR		m_trajectTable[MAXTRAJECT];
};


#endif //_TERRAIN_H_