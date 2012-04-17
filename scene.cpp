#include "scene.h"

Scene::Scene()
{  }

Scene::~Scene() {
  for( LightSet::iterator i = _lights.begin(); i != _lights.end(); i++ )
    delete (*i);
  for( ShapeSet::iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    delete (*i);
}

void Scene::add_shape( Shape* s )  { _shapes.insert(s); }
void Scene::add_light( Light* l )  { _lights.insert(l); }
void Scene::set_cam( const Camera& cam )  { _cam = cam; }
