#ifndef MESH_COMPONENT
#define MESH_COMPONENT

#include "Cross.h"
#include "ComponentView.h"
#include "Mesh.h"

class QLabel;
class QLineEdit;
class QCheckBox;

using namespace cross;

class MeshComponent : public ComponentView<Mesh>
{
	Q_OBJECT
public:
	MeshComponent(QWidget* parent = 0) : ComponentView(parent) { }
	~MeshComponent() override { }

	void Initialize() override;
	void Show(Mesh* e) override;

private:
	QLabel* model_value			= NULL;
	QLabel* poly_count_value	= NULL;
	QLineEdit* material_value	= NULL;
	QCheckBox* face_culling_box = NULL;

	void OnLoadClick();
};

#endif