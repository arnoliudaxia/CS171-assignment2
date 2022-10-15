#include <bezier.h>
#include <utils.h>
#include <vector>

BezierCurve::BezierCurve(int m) { control_points_.resize(m); }

BezierCurve::BezierCurve(std::vector<vec3>& control_points) {
  control_points_ = control_points;
}

void BezierCurve::setControlPoint(int i, vec3 point) {
  control_points_[i] = point;
}


Vertex BezierCurve::evaluate(std::vector<vec3>& control_points, float t) {
	std::vector<vec3> nowControlPoints = control_points;
	Vertex result;
	while (nowControlPoints.size() > 1) {
		std::vector<vec3> nextControlPoints;
		for (int i = 0; i < nowControlPoints.size() - 1; i++) {
			vec3 nextPoint = (1 - t) * nowControlPoints[i] + t * nowControlPoints[i + 1];
			nextControlPoints.push_back(nextPoint);
		}
		if (nextControlPoints.size() == 2)
		{
			result.normal = normalize(nextControlPoints[1] - nextControlPoints[0]);
		}
		nowControlPoints = nextControlPoints;
	}
	result.position = nowControlPoints[0];
	return result;
}

Vertex BezierCurve::evaluate(float t) {
  return evaluate(control_points_, t);
}

/// <summary>
/// 贝塞尔曲线采样6个点，画折线
/// </summary>
/// <returns></returns>
Object BezierCurve::generateObject() {
	Object result;
	//贝塞尔采样5个点
	for (int i = 0; i < 10; i++) {
		result.vertices.push_back(evaluate(0.2*i));
	}
	result.draw_mode.primitive_mode = GL_LINES;
	
	return result;
}

BezierSurface::BezierSurface(int m, int n) {
  control_points_m_.resize(m);
  for (auto& sub_vec : control_points_m_) {
    sub_vec.resize(n);
  }
  control_points_n_.resize(n);
  for (auto& sub_vec : control_points_n_) {
    sub_vec.resize(m);
  }
}

/**
 * @param[in] i: index (i < m)
 * @param[in] j: index (j < n)
 * @param[in] point: the control point with index i, j
 */
void BezierSurface::setControlPoint(int i, int j, vec3 point) {
  control_points_m_[i][j] = point;
  control_points_n_[j][i] = point;
}


/// <summary>
/// evaluate the point at (u, v) with the given control points
/// </summary>
/// <param name="u"></param>
/// <param name="v"></param>
/// <returns>P(u,v)对应的点</returns>
Vertex BezierSurface::evaluate(float u, float v) {
	std::vector<vec3> controlPoints;
	
  //先在u方向的贝塞尔曲线取参数u
	for (size_t i = 0; i < 4; i++)
	{
		BezierCurve bc(control_points_m_[i]);
		controlPoints.push_back(bc.evaluate(u).position);
	}
	//由这四个控制点在v方向上做贝塞尔参数化
	BezierCurve bc(controlPoints);
	Vertex dv = bc.evaluate(v);
	
	//从另一个方向上再做一遍
	controlPoints.clear();
	for (size_t i = 0; i < 4; i++)
	{
		BezierCurve bc(control_points_n_[i]);
		controlPoints.push_back(bc.evaluate(v).position);
	}
	BezierCurve bc2(controlPoints);
	Vertex du = bc2.evaluate(u);
	Vertex result;
	result.position = dv.position;
	result.normal = cross(du.normal, dv.normal);
	return result;

}

void poitnfix(std::vector<Vertex>& result,Vertex v[3])
{
	result.push_back(v[0]);
	result.push_back(v[1]);
	result.push_back(v[2]);
	return;
	float cut = -2.7f;
	int insideNum=0, outsideNum = 0;
	for (size_t i = 0; i < 3; i++)
	{
		if (v[i].position.z < cut)
		{
			insideNum++;
		}
		else
		{
			outsideNum++;
		}
	}
	if (outsideNum == 0)
	{
		result.push_back(v[0]);
		result.push_back(v[1]);
		result.push_back(v[2]);

	}
	if (insideNum > 0 && outsideNum > 0)
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (v[i].position.z > cut)
			{
				v[i].position.z = cut;
			}
		}
		result.push_back(v[0]);
		result.push_back(v[1]);
		result.push_back(v[2]);
	}

}

/// <summary>
/// 通过贝塞尔曲面生成mesh点
/// </summary>
/// <returns></returns>
std::vector<Vertex> BezierSurface::generateObject() {
	std::vector<Vertex> result;
	std::vector<std::vector<Vertex>> grid;
	grid.resize(6);
	for (size_t i = 0; i < 6; i++)
	{
		grid[i].resize(6);
	}
	//贝塞尔采样6x6的网格
	for (int i = 0; i <= 5; i++) {
		for (size_t j = 0; j <= 5; j++)
		{
			grid[i][j] = evaluate(0.2 * i, 0.2 * j);
		}
		
	}
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			float cut = -1.5f;
			//每一个square对应两个三角
			Vertex v[3] = { grid[i][j] ,grid[i][j + 1],grid[i + 1][j] };
			poitnfix(result, v);
			//result.push_back(grid[i][j]);
			//result.push_back(grid[i][j + 1]);
			//result.push_back(grid[i + 1][j]);
			Vertex vv[3] = { grid[i][j+1] ,grid[i+1][j],grid[i + 1][j+1] };
			poitnfix(result, vv);

			//result.push_back(grid[i][j + 1]);
			//result.push_back(grid[i + 1][j]);
			//result.push_back(grid[i + 1][j + 1]);

		}
	}

	return result;

}

/**
 * TODO: read in bzs file to generate a vector of Bezier surface
 * for the first line we have b p m n
 * b means b bezier surfaces, p means p control points.
 *
 */
std::vector<BezierSurface> read(const std::string &path) {
    printf("Loading OBJ file %s...\n", path.c_str());
	FILE* file = fopen(path.c_str(), "r");
	if (file == NULL) {
		printf("路径不正确\n");
		getchar();
		assert(1!=1);
		return std::vector<BezierSurface>();
	}
	int faceNum, vertexNumber, rowN, colN;
	fscanf(file, "%d %d %d %d", &faceNum,&vertexNumber,&rowN,&colN);
	char lineHeader[128];
	std::vector<std::vector<int>> indexs;
	
	//读取面索引
	for (size_t i = 0; i < faceNum; i++)
	{
		std::vector<int> indexLine;
		int index;
		fscanf(file, "%d", &index);
		indexLine.push_back(index);
		for (size_t facePoint = 0; facePoint < rowN*colN-1; facePoint++)
		{
			fscanf(file, " %d", &index);
			indexLine.push_back(index);

		}

		indexs.push_back(indexLine);
	}
	//读取顶点坐标
	std::vector<vec3> vertexs;
	for (size_t i = 0; i < vertexNumber; i++)
	{
		vec3 vertex;
		fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
		vertexs.push_back(vertex);
	}
	//坐标取代索引
	std::vector<BezierSurface> BS;
	for (size_t i = 0; i < faceNum; i++)
	{
		BezierSurface bs(rowN, colN);
		for (size_t j = 0; j < rowN; j++)
		{
			for (size_t k = 0; k < colN; k++)
			{
				bs.setControlPoint(j, k, vertexs[indexs[i][j * colN + k]]);
			}
		}
		BS.push_back(bs);
	}
	return BS;
	
}