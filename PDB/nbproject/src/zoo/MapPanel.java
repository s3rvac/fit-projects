/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief ZOO map panel class.
 *
 */

package zoo;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.TexturePaint;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/**
 * @brief ZOO map containing drawn objects.
 */
public class MapPanel extends javax.swing.JPanel {

	/// Back buffer.
	private BufferedImage _backBuffer;

	/// Front buffer.
	private BufferedImage _frontBuffer;

	/// Background color.
	private static final Color BACKGROUND_COLOR = new Color(238, 238, 238);

	/**
	 * @brief Map modes.
	 */
	public enum MapMode {
		MAP, BUILDING, ANIMAL
	}

	/// Selected map mode.
	private MapMode _mapMode = MapMode.MAP;

	/**
	 * @brief Mouse modes.
	 */
	public enum MouseMode {
		DEFAULT, MOVING, INSERTING, DRAGGING
	}

	/// Selected mouse mode.
	private MouseMode _mouseMode = MouseMode.DEFAULT;

	/**
	 * @brief Default constructor.
	 */
	public MapPanel() {
		initComponents();
	}

	/**
	 * @brief Draws background according to the selected date.
	 *
	 * @param[in] date Selected date.
	 */
	private void drawBackground(Date date) {
		try {
			String seasonsDir = System.getProperty("user.dir") +
					"/src/zoo/resources/seasons/";
			String seasonsFile = null;

			switch (date.getMonth() + 1) {
				case 11:
				case 12:
				case 1:
				case 2:
					seasonsFile = seasonsDir + "winter.png";
					break;
				case 3:
				case 4:
					seasonsFile = seasonsDir + "spring.png";
					break;
				case 5:
				case 6:
				case 7:
				case 8:
					seasonsFile = seasonsDir + "summer.png";
					break;
				case 9:
				case 10:
					seasonsFile = seasonsDir + "autumn.png";
					break;
				default:
					seasonsFile = seasonsDir + "summer.png";
					break;
			}

			BufferedImage background = ImageIO.read(new File(seasonsFile));
			TexturePaint backgroundTexture = new TexturePaint(background,
					new Rectangle(0,0,background.getWidth(),background.getHeight()));
			Graphics2D g2 = (Graphics2D)_backBuffer.getGraphics();
			g2.setPaint(backgroundTexture);
			g2.fill(new Rectangle(0,0,this.getWidth(),this.getHeight()));
		}
		catch (IOException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}
	}

	/**
	 * @brief Initializes drawing.
	 *
	 * This method initializes the back buffer, and it needs to be called before
	 * drawing anything.
	 *
	 * @param[in] date Current date.
	 */
	public void startDrawing(Date date) {
		_backBuffer = new BufferedImage(getWidth(), getHeight(),
				BufferedImage.TYPE_INT_ARGB);

		if (_mapMode != MapMode.MAP) {
			drawBackground(date);
		}
	}

	/**
	 * @brief Ends drawing.
	 *
	 * This method ends drawing into a back buffer and switches the front buffer
	 * with the back buffer.
	 */
	public void endDrawing() {
		// Swap the back buffer with the front buffer
		_frontBuffer = _backBuffer;
		_backBuffer = null;

		// Invalidate the client area
		repaint();
	}

	@Override
	public void paint(Graphics g) {
		// Call the predecesor
		super.paint(g);

		if (_frontBuffer != null) {
			g.drawImage(_frontBuffer, 0, 0, null);
		}
	}

	/**
	* @brief Function that save back buffer to some components.
	*        This saved back buffer is used to real-time redraw.
	*/
	public void saveBackBuffer() {
		DragAndDrop.getInstance().saveBuffer(_backBuffer);
		Insert.getInstance().saveBuffer(_backBuffer);
	}

	/**
	* @brief Sets a new map mode.
	*
	* @param[in] m New map mode.
	*/
	public void setMapMode(MapMode m) {
		_mapMode = m;
	}

	/**
	* @brief Sets a new mouse mode.
	*
	* @param[in] m New mouse mode.
	*/
	public void setMouseMode(MouseMode m) {
		_mouseMode = m;
	}

	/**
	* @brief Gets a mouse mode.
	* @return Mouse mode.
	*/
	public MouseMode getMouseMode() {
		return _mouseMode;
	}

	/**
	 * @brief Performs some operation on the selected object (draw, fill, ...).
	 *
	 * @param[in] drawnObject Object to be drawn.
	 * @param[in] fill Should the object be filled?
	 */
	private void doDrawObject(ObjectData drawnObject, boolean fill) {
		int objectType = drawnObject.objectType;
		int []objectCoords = View.getInstance().db2map(drawnObject.coordinates);
		Color objectColor = drawnObject.color;

		Graphics graphics = _backBuffer.getGraphics();
		graphics.setColor(objectColor);

		switch (objectType) {
			case ObjectData.POINT_TYPE: { // point
				final int POINT_SIZE = 6;
				int x = objectCoords[0] - POINT_SIZE / 2;
				int y = objectCoords[1] - POINT_SIZE / 2;

				// First fill the object either with its color or with
				// the background color...
				if (fill) {
					graphics.fillRect(x, y, POINT_SIZE, POINT_SIZE);
				} else {
					graphics.setColor(BACKGROUND_COLOR);
					graphics.fillRect(x, y, POINT_SIZE, POINT_SIZE);
				}

				// ... then draw the object borders
				graphics.setColor(objectColor);
				graphics.drawRect(x, y, POINT_SIZE, POINT_SIZE);
				break;
			}

			case ObjectData.POLYGON_TYPE: { // polygon
				final int NUMBER_OF_POINTS = objectCoords.length / 2;
				int[] xPoints = new int[NUMBER_OF_POINTS];
				int[] yPoints = new int[NUMBER_OF_POINTS];
				for (int i = 0,  j = 0; i < (objectCoords.length - 1); i = i + 2, ++j) {
					xPoints[j] = objectCoords[i];
					yPoints[j] = objectCoords[i + 1];
				}

				// First fill the object either with its color or with
				// the background color...
				if (fill) {
					graphics.fillPolygon(xPoints, yPoints, NUMBER_OF_POINTS);
				} else {
					graphics.setColor(BACKGROUND_COLOR);
					graphics.fillPolygon(xPoints, yPoints, NUMBER_OF_POINTS);
				}

				// ... then draw the object borders
				graphics.setColor(objectColor);
				graphics.drawPolygon(xPoints, yPoints, NUMBER_OF_POINTS);
				break;
			}

			case ObjectData.LINE_TYPE: { // line
				if (objectCoords.length == 4) {
					graphics.drawLine(objectCoords[0], objectCoords[1],
							objectCoords[2], objectCoords[3]);
				}
				break;
			}

			case ObjectData.RECTANGLE_TYPE: { // rectangle
				int x = objectCoords[0];
				int y = objectCoords[1];
				int width = objectCoords[2] - objectCoords[0];
				int height = objectCoords[3] - objectCoords[1];

				// First fill the object either with its color or with
				// the background color...
				if (fill) {
					graphics.fillRect(x, y, width, height);
				} else {
					graphics.setColor(BACKGROUND_COLOR);
					graphics.fillRect(x, y, width, height);
				}

				// ... then draw the object borders
				graphics.setColor(objectColor);
				graphics.drawRect(x, y, width, height);
				break;
			}

			case ObjectData.CIRCLE_TYPE: { // circle
				if (objectCoords.length >= 4) {
					int centreX = objectCoords[0];
					int centreY = objectCoords[3];
					int radius = Math.abs(centreY - objectCoords[1]);

					// First fill the object either with its color or with
					// the background color...
					if (fill) {
						graphics.fillArc(centreX - radius, centreY - radius, (int) radius * 2,
								(int) radius * 2, 0, 360);
					} else {
						graphics.setColor(BACKGROUND_COLOR);
						graphics.fillArc(centreX - radius, centreY - radius, (int) radius * 2,
								(int) radius * 2, 0, 360);
					}

					// ... then draw the object borders
					graphics.setColor(objectColor);
					graphics.drawArc(centreX - radius, centreY - radius, (int) radius * 2,
								(int) radius * 2, 0, 360);
				}
				break;
			}

			default:
				Logger.getLogger("zoo").log(Level.WARNING,
						"Unknown object type in switch.");
				break;
		}

		if (_mapMode != MapMode.MAP) {
			Envelope envelope = new Envelope(drawnObject);

			if (_mapMode == MapMode.BUILDING) {
				envelope.drawPhoto(_backBuffer.getGraphics());
			}
			else {
				envelope.drawAnimals(_backBuffer.getGraphics());
			}
		}
	}

	/**
	* @brief Map mouse pressed handle code.
	*
	* @param[in] mapPosition Mouse point position in a map.
	*/
	public void mousePressed(Point mapPosition) {
		if (_mapMode == MapMode.ANIMAL) {
			DragAndDrop.getInstance().mousePressed(mapPosition);
		}
	}

	/**
	* @brief Map mouse dragged handle code.
	*
	* @param[in] mapPosition Mouse point position in a map.
	*/
	public void mouseDragged(Point mapPosition) {
		if (_mapMode == MapMode.ANIMAL) {
			startDrawing(Calendar.getInstance().getDate());
			DragAndDrop.getInstance().mouseDragged(mapPosition, _backBuffer.getGraphics());
			endDrawing();
		}
	}

	/**
	* @brief Map mouse released handle code.
	*
	* @param[in] mapPosition Mouse point position in a map.
	* @param[in] zv Reference to ZooView.
	*/
	public void mouseReleased(Point mapPosition, ZooView zv) {
		if (_mapMode == MapMode.ANIMAL) {
			startDrawing(Calendar.getInstance().getDate());
			DragAndDrop.getInstance().mouseReleased(mapPosition, _backBuffer.getGraphics());
			endDrawing();
		}
		zv.redrawPanels();
	}

	/**
	* @brief Map mouse moved insert handle code.
	*
	* @param[in] mapPosition Mouse point position in a map.
	*/
	public void mouseMovedInsert(Point mapPosition) {
		startDrawing(Calendar.getInstance().getDate());
		Insert.getInstance().movePoint(mapPosition, _backBuffer.getGraphics(), this);
		endDrawing();
	}

	/**
	* @brief Map mouse clicked insert handle code.
	*
	* @param[in] mapPosition Mouse point position in a map.
	*/
	public void mouseClickedInsertAdd(Point mapPosition) {
		startDrawing(Calendar.getInstance().getDate());
		Insert.getInstance().addPoint(mapPosition, _backBuffer.getGraphics(), this);
		endDrawing();
	}

	/**
	* @brief Map mouse clicked insert last point (polygons) handle code.
	*
	* @param[in] mapPosition Mouse point position in a map.
	*/
	public void mouseClickedInsertLast(Point mapPosition) {
		startDrawing(Calendar.getInstance().getDate());
		Insert.getInstance().lastPoint(mapPosition, _backBuffer.getGraphics(), this);
		endDrawing();
	}

	/**
	 * @brief Fills the selected object with the selected color.
	 *
	 * @param[in] drawnObject Object to be drawn.
	 */
	public void fillObject(ObjectData drawnObject) {
		doDrawObject(drawnObject, true);
	}

	/**
	 * @brief Clears the fill of the selected object.
	 *
	 * @param[in] drawnObject Object to be drawn.
	 *
	 * Object will be filled with the background color.
	 */
	public void clearObjectFill(ObjectData drawnObject) {
		doDrawObject(drawnObject, false);
	}

	/**
	 * @brief Removes the selected object.
	 *
	 * @param[in] drawnObject Object to be drawn.
	 */
	public void removeObject(ObjectData drawnObject) {
		drawnObject.color = BACKGROUND_COLOR;
		doDrawObject(drawnObject, true);
	}

	/**
	 * @brief Draws the selected object.
	 *
	 * @param[in] drawnObject Object to be drawn.
	 */
	public void drawObject(ObjectData drawnObject) {
		doDrawObject(drawnObject, false);
	}

	/** This method is called from within the constructor to
	 * initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is
	 * always regenerated by the Form Editor.
	 */
	@SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        setBorder(javax.swing.BorderFactory.createEtchedBorder());
        setName("Form"); // NOI18N

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 396, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 296, Short.MAX_VALUE)
        );
    }// </editor-fold>//GEN-END:initComponents

    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables
}
