import QtQuick 2.3
import Ubuntu.Components 1.3

MultiPointTouchArea {
	id: root
	property int unit: width / 19
	property int bsize: unit * 9

	property color gb_white_accent: "#EDEDED"
	property color gb_purple: "#B01561"
	property color gb_purple_accent: Qt.darker("#CF2463", 1.4)
	property real outline: units.gu(0.375)

	
	Rectangle {
		id: a

		x: bsize
		y: 0
		width: bsize
		height: bsize 
		radius: bsize / 2

		color: gb_purple
		border.color: gb_purple_accent
		border.width: outline

		Label {
			id: alabel
			anchors.centerIn: parent
			color: gb_white_accent
			text: "A"
			fontSize: "x-large"
			font.bold: true
		}
	}

	Rectangle {
		id: b
		x: 0
		y: bsize / 2
		width: bsize
		height: bsize
		radius: bsize / 2
		color: a.color 
		border.color: a.border.color
		border.width: a.border.width
	
		Label {
			id: blabel
			anchors.centerIn: parent
			text: "B"
			color: gb_white_accent
			fontSize: "x-large"
			font.bold: true
		}
	}

	property bool aIsDown: false
	property bool bIsDown: false

	signal aPressed()
	signal bPressed()
	signal aReleased()
	signal bReleased()

	onTouchUpdated: {
		var r = a.radius;
		var ax = a.x + r;
		var bx = b.x + r;
		var by = b.y + r;
		var ay = a.y + r;
		var r2 = r * r;

		var aDown = false;
		var bDown = false;

		for (var i in touchPoints) {
			var pt = touchPoints[i];
			if (pt.pressed) {
				var dax = ax - pt.x;
				var day = ay - pt.y;
				var dbx = bx - pt.x;
				var dby = by - pt.y;

				if (dax * dax + day * day <= r2) {
					aDown = true;
				} 

				if (dbx * dbx + dby * dby <= r2) {
					bDown = true;
				}
			}
		}

		if (aDown != aIsDown) {
			if (!aDown) {
				aReleased();
			} else if (!aIsDown) {
				aPressed();
			}
			aIsDown = aDown;
		}

        if (bDown != bIsDown) {
			if (!bDown) {
				bReleased();
			} else if (!bIsDown) {
				bPressed();
			}
			bIsDown = bDown;
		}
	}
}
