/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QSignalMapper>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>

#include <qrutils/qRealDialog.h>
#include <qrutils/graphicsUtils/lineImpl.h>
#include <kitBase/readOnly.h>

#include <kitBase/devicesConfigurationWidget.h>
#include <kitBase/devicesConfigurationProvider.h>

#include "twoDModel/engine/twoDModelDisplayWidget.h"

#include "twoDModel/twoDModelDeclSpec.h"

class QComboBox;
class QDomDocument;

namespace Ui {
class D2Form;
}

namespace graphicsUtils {
class AbstractItem;
}

namespace twoDModel {

namespace model {
class Model;
class RobotModel;
}

namespace view {

class D2ModelScene;
class SensorItem;
class RobotItem;

class TWO_D_MODEL_EXPORT D2ModelWidget : public utils::QRealDialog, public kitBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	D2ModelWidget(model::Model &model, QWidget *parent = 0);
	~D2ModelWidget();

	void init();
	void close();

	D2ModelScene *scene();
	engine::TwoDModelDisplayWidget *display();

	SensorItem *sensorItem(const kitBase::robotModel::PortInfo &port);
	void setSensorVisible(const kitBase::robotModel::PortInfo &port, bool isVisible);

	void loadXml(const QDomDocument &worldModel);

	/// Returns a reference to a model part of 2D model MVC architecture.
	model::Model &model() const;

	/// Sets groups of items on 2d model that can not be modified by user. Used for "challenge" mode where student
	/// shall provide program that makes robot do specific task in given unmodifyable world model.
	/// @see ReadOnly
	void setInteractivityFlags(kitBase::ReadOnlyFlags flags);

signals:
	/// Emitted each time when user closes 2D model window.
	void widgetClosed();

	void robotWasIntersectedByWall(bool isNeedStop, const QPointF &oldPos);

	/// Emitted when such features as motor or sensor noise were
	///enabled or disabled by user
	void noiseSettingsChanged();

	/// Emitted when user has started intepretation from the 2D model window.
	void runButtonPressed();

	/// Emitted when user has stopped intepretation from the 2D model window.
	void stopButtonPressed();

protected:
	void changeEvent(QEvent *e) override;
	void showEvent(QShowEvent *e) override;
	void keyPressEvent(QKeyEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

	void onDeviceConfigurationChanged(const QString &robotModel
			, const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &device
			, Reason reason) override;

private slots:
	void bringToFront();

	void saveToRepo();
	void saveWorldModel();
	void loadWorldModel();

	void changePenWidth(int width);
	void changePenColor(int textIndex);
	void onSelectionChange();

	void changeSpeed(int curIndex);

	void enableRobotFollowing(bool on);
	void onHandCursorButtonToggled(bool on);
	void onMultiselectionCursorButtonToggled(bool on);

	void changePhysicsSettings();

	void toggleDisplayVisibility();

	void returnToStartMarker();

private:
	enum CursorType
	{
		noDrag = 0
		, hand
		, multiselection
		, drawWall
		, drawLine
		, drawStylus
		, drawEllipse
	};

	static const int defaultPenWidth = 6;

	static const int indexOfNoneSensor = 0;
	static const int indexOfTouchSensor = 1;
	static const int indexOfColorSensor = 2;
	static const int indexOfSonarSensor = 3;
	static const int indexOfLightSensor = 4;

	struct RobotState {
	public:
		RobotState();

		QPointF pos;
		double rotation;
	};

	void changePalette();
	void connectUiButtons();
	void initButtonGroups();
	void setPortsGroupBoxAndWheelComboBoxes();
	void unsetPortsGroupBoxAndWheelComboBoxes();

	void setHighlightOneButton(QAbstractButton * const oneButton);

	void setDisplayVisibility(bool visible);

	QDomDocument generateXml() const;

	/// Set active panel toggle button and deactivate all others
	void setActiveButton(int active);

	/// Get QPushButton for current sensor
	QPushButton *currentPortButton();

	/// Reread sensor configuration on given port, delete old sensor item and create new.
	void reinitSensor(RobotItem *robotItem, const kitBase::robotModel::PortInfo &port);

	void setValuePenColorComboBox(const QColor &penColor);
	void setValuePenWidthSpinBox(int width);
	void setItemPalette(const QPen &penItem, const QBrush &brushItem);

	void setCursorTypeForDrawing(CursorType type);
	void setCursorType(CursorType cursor);

	void initWidget();
	QList<graphicsUtils::AbstractItem *> selectedColorItems() const;
	bool isColorItem(graphicsUtils::AbstractItem * const item) const;

	void centerOnRobot();
	QGraphicsView::DragMode cursorTypeToDragType(CursorType type) const;
	QCursor cursorTypeToCursor(CursorType type) const;
	void refreshCursor();
	void syncCursorButtons();

	void onFirstShow();

	void initRunStopButtons();

	void updateWheelComboBoxes();

	void onRobotListChange(RobotItem *robotItem);

	void setSelectedRobotItem(RobotItem *robotItem);
	void unsetSelectedRobotItem();

	void incrementTimelineCounter();

	Ui::D2Form *mUi = nullptr;
	D2ModelScene *mScene = nullptr;

	RobotItem *mSelectedRobotItem = nullptr;
	kitBase::DevicesConfigurationWidget *mCurrentConfigurer;

	model::Model &mModel;

	engine::TwoDModelDisplayWidget *mDisplay = nullptr;

	int mWidth = 0;

	QButtonGroup mButtonGroup;
	QButtonGroup mCursorButtonGroup;

	CursorType mNoneCursorType; // cursorType for noneStatus
	CursorType mCursorType; // current cursorType

	bool mFollowRobot = false;

	bool mFirstShow = true;

	bool mDisplayIsVisible = false;

	bool mAutoOpen;

	QString mToolsTabName;
	QString mModelSettingsTabName;
	QString mPortsTabName;

	bool mSensorsReadOnly = false;
};

}
}
