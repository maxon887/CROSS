#include "MaterialView.h"
#include "File.h"
#include "System.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Game.h"
#include "Scene.h"

#include "Libs/TinyXML2/tinyxml2.h"

#include <iomanip>

#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QFileDialog>

using namespace cross;
using namespace tinyxml2;

MaterialView::~MaterialView(){
}

void MaterialView::Initialize(){
	shader_label = findChild<QLabel*>("shaderLabel");
	shader_edit = findChild<QLineEdit*>("shaderEdit");
	properties_box = findChild<QGroupBox*>("properties");
	QPushButton* loadBtn = findChild<QPushButton*>("loadBtn");
	connect(loadBtn, &QPushButton::clicked, this, &MaterialView::OnLoadShaderClick);

	color_dialog = new QColorDialog(dynamic_cast<QPushButton*>(this));
	color_dialog->setWindowFlags(color_dialog->windowFlags() | Qt::WindowStaysOnTopHint);
	connect(color_dialog, &QColorDialog::currentColorChanged, this, &MaterialView::OnCurrentColorChanged);
	connect(color_dialog, &QColorDialog::colorSelected, this, &MaterialView::OnColorSelected);
	connect(color_dialog, &QColorDialog::rejected, this, &MaterialView::OnColorRejected);

	apply_btn = findChild<QPushButton*>("applyBtn");
	apply_btn->setDisabled(true);
	connect(apply_btn, &QPushButton::clicked, this, &MaterialView::OnApplyClick);
	revert_btn = findChild<QPushButton*>("revertBtn");
	revert_btn->setDisabled(true);
	connect(revert_btn, &QPushButton::clicked, this, &MaterialView::OnRevertClick);
}

void MaterialView::OnEntitySelected(Entity* e){
	color_dialog->hide();
	PropertyView::OnEntitySelected(e);
}

void MaterialView::OnFileSelected(const string& filepath){
	if(File::ExtensionFromFile(filepath) != "mat") {
		color_dialog->hide();
		PropertyView::OnFileSelected(filepath);
		return;
	}
	Clear();

	string filename = File::FileFromPath(File::FileWithoutExtension(filepath));
	setTitle(QString("Material: ") + filename.c_str());

	material = game->GetCurrentScene()->GetMaterial(filepath);
	delete original;
	original = material->Clone();
	if(material->GetShader()){
		shader_edit->setText(material->GetShader()->GetFilename().c_str());
	}else{
		shader_edit->clear();
	}

	RefreshProperties();

	apply_btn->setDisabled(true);
	revert_btn->setDisabled(true);
	show();
}

void MaterialView::Clear(){
	QWidget* propertyLayout = NULL;
	do {
		delete propertyLayout;
		propertyLayout = properties_box->findChild<QWidget*>("propertyLayout");
	} while(propertyLayout);
}

void MaterialView::RefreshProperties() {
	for(const Shader::Property& prop : material->GetProperties()) {
		QWidget* propLayout = CreateProperty(prop.GetName(), prop.GetType());
		switch(prop.GetType()) {
		case Shader::Property::Type::TEXTURE: {
			break;
		}
		case Shader::Property::Type::INT:
			break;
		case Shader::Property::Type::FLOAT:
			break;
		case Shader::Property::Type::COLOR: {
			QPushButton* colorPicker = propLayout->findChild<QPushButton*>("colorPicker");
			QString colorStyle = "background-color: ";
			const Color& c = prop.GetValue().color;
			QColor qColor(c.R * 255, c.G * 255, c.B * 255, c.A * 255);
			colorPicker->setStyleSheet(colorStyle + QColor(c.R * 255, c.G * 255, c.B * 255, c.A * 255).name());

			QLineEdit* valueBox = propLayout->findChild<QLineEdit*>("valueBox");
			valueBox->setText(GetColorStr(c).c_str());
			break;
		}
		default:
			break;
		}
	}
}

void MaterialView::OnValueChanged(){
	QWidget* parent = dynamic_cast<QWidget*>(sender()->parent());
	QLabel* nameLabel = parent->findChild<QLabel*>("nameLabel");
	
	Shader::Property* prop = material->GetProperty(nameLabel->text().toStdString());
	switch(prop->GetType())	{
	case Shader::Property::COLOR: {
		QLineEdit* valueBox = parent->findChild<QLineEdit*>("valueBox");
		Color c(valueBox->text().toStdString().c_str());
		prop->SetValue(c);
		break;
	}
	default:
		CROSS_FAIL(false, "Unsupported property type");
	}

	apply_btn->setDisabled(false);
	revert_btn->setDisabled(false);
}

void MaterialView::OnSomethingChanged() {
	apply_btn->setDisabled(false);
	revert_btn->setDisabled(false);
}

QWidget* MaterialView::CreateProperty(const string& name, Shader::Property::Type type) {
	QWidget* propertyLayoutWidget = new QWidget(properties_box);
	propertyLayoutWidget->setObjectName("propertyLayout");
	QHBoxLayout* propertyLayout = new QHBoxLayout(propertyLayoutWidget);
	propertyLayout->setSpacing(12);
	propertyLayout->setMargin(0);

	QLabel* propertyNameLabel = new QLabel(propertyLayoutWidget);
	propertyNameLabel->setObjectName("nameLabel");
	propertyNameLabel->setText(name.c_str());
	propertyNameLabel->setFixedWidth(250);
	propertyLayout->addWidget(propertyNameLabel);

	switch(type) {
	case cross::Shader::Property::TEXTURE: {
		QLineEdit* filename = new QLineEdit(propertyLayoutWidget);
		filename->setObjectName("filename");
		propertyLayout->addWidget(filename);
		break;
	}
	case cross::Shader::Property::FLOAT: {
		QSlider* valueSlider = new QSlider(propertyLayoutWidget);
		valueSlider->setOrientation(Qt::Horizontal);
		propertyLayout->addWidget(valueSlider);

		QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
		valueBox->setObjectName("valueBox");
		propertyLayout->addWidget(valueBox);
		break;
	}
	case cross::Shader::Property::INT: {
		QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
		valueBox->setObjectName("valueBox");
		propertyLayout->addWidget(valueBox);
		break;
	}
	case cross::Shader::Property::COLOR: {
		QPushButton* colorPicker = new QPushButton(propertyLayoutWidget);
		colorPicker->setObjectName("colorPicker");
		colorPicker->setFixedWidth(60);
		colorPicker->setFixedHeight(31);
		connect(colorPicker, &QPushButton::clicked, this, &MaterialView::OnColorPickerClicked);
		propertyLayout->addWidget(colorPicker);

		QLineEdit* valueBox = new QLineEdit(propertyLayoutWidget);
		valueBox->setObjectName("valueBox");
		valueBox->setInputMask("HHHHHHHH");
		valueBox->setFixedWidth(100);
		connect(valueBox, &QLineEdit::returnPressed, this, &MaterialView::OnValueChanged);
		connect(valueBox, &QLineEdit::textChanged, this, &MaterialView::OnSomethingChanged);
		propertyLayout->addWidget(valueBox);

		QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);
		propertyLayout->addItem(spacer);
		break;
	}
	default:
		break;
	}

	QVBoxLayout* groupBoxLayout = dynamic_cast<QVBoxLayout*>(properties_box->layout());
	groupBoxLayout->insertWidget(groupBoxLayout->count() - 1, propertyLayoutWidget);

	propertyLayoutWidget->show();

	return propertyLayoutWidget;
}

void MaterialView::OnLoadShaderClick() {
	QString path = QDir::currentPath() + "/" + QString(system->AssetsPath().c_str());
	QString filePath = QFileDialog::getOpenFileName(this, "Select Shader File", path, "Shader File (*.sha)");
	if(!filePath.isEmpty()) {
		QDir root = path;
		QString filepath = root.relativeFilePath(filePath);
		shader_edit->setText(filepath);
		Shader* newShader = game->GetCurrentScene()->GetShader(filepath.toStdString().c_str());
		material->SetShader(newShader);
		RefreshProperties();
	}
}

void MaterialView::OnApplyClick() {
	QList<QWidget*> properties = properties_box->findChildren<QWidget*>("propertyLayout");
	for(QWidget* propertyL : properties) {
		QLabel* nameLabel = propertyL->findChild<QLabel*>("nameLabel");
		Shader::Property* prop = material->GetProperty(nameLabel->text().toStdString());
		switch(prop->GetType()) {
		case Shader::Property::Type::COLOR: {
			QLineEdit* valueBox = propertyL->findChild<QLineEdit*>("valueBox");
			if(valueBox->hasAcceptableInput()) {
				prop->SetValue(Color(valueBox->text().toStdString().c_str()));
			}
			break;
		}
		default:
			break;
		}
	}

	material->Save(system->AssetsPath() + material->GetFilename());
	OnFileSelected(material->GetFilename());
}

void MaterialView::OnRevertClick() {
	material->SetShader(original->GetShader());
	material->EnableTransparency(original->IsTransparent());
	Array<Shader::Property>& originalProps = original->GetProperties();
	Array<Shader::Property>& materialProps = material->GetProperties();
	materialProps = originalProps;
	OnFileSelected(string(material->GetFilename()));
}

void MaterialView::OnColorPickerClicked(){
	current_property_layout = dynamic_cast<QWidget*>(sender()->parent());
	QLabel* nameLabel = current_property_layout->findChild<QLabel*>("nameLabel");
	current_property = material->GetProperty(nameLabel->text().toStdString());

	const Color& c = current_property->GetValue().color;
	QColor qcolor(c.R * 255, c.G * 255, c.B * 255, c.A * 255);
	color_dialog->setCurrentColor(qcolor);
	color_dialog->move(mapToGlobal(nameLabel->pos()));
	color_dialog->show();
	color_dialog->raise();
}

void MaterialView::OnCurrentColorChanged(const QColor& c){
	if(current_property){
		Color color(c.red(), c.green(), c.blue(), c.alpha());
		current_property->SetValue(color);
	}
}

void MaterialView::OnColorSelected(const QColor& c){
	if(current_property) {
		Color color(c.red(), c.green(), c.blue(), c.alpha());
		current_property->SetValue(color);

		QPushButton* colorPicker = current_property_layout->findChild<QPushButton*>("colorPicker");
		QString colorStyle = "background-color: ";
		colorPicker->setStyleSheet(colorStyle + c.name());

		QLineEdit* valueBox = current_property_layout->findChild<QLineEdit*>("valueBox");
		valueBox->setText(GetColorStr(color).c_str());

		current_property_layout = NULL;
		current_property = NULL;
	}
}

void MaterialView::OnColorRejected(){
	QLineEdit* valueBox = current_property_layout->findChild<QLineEdit*>("valueBox");
	Color c(valueBox->text().toStdString().c_str());
	current_property->SetValue(c);
	current_property_layout = NULL;
	current_property = NULL;
}

string MaterialView::GetColorStr(const Color& c){
	int rInt = c.R * 255;
	int gInt = c.G * 255;
	int bInt = c.B * 255;
	int aInt = c.A * 255;

	std::stringstream ss;
	ss << std::hex;
	ss << std::setw(2) << std::setfill('0') << rInt;
	ss << std::setw(2) << std::setfill('0') << gInt;
	ss << std::setw(2) << std::setfill('0') << bInt;
	ss << std::setw(2) << std::setfill('0') << aInt;
	string text = ss.str();
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);

	return text;
}