/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Application main frame class.
 *
 */

package zoo;

import java.awt.Cursor;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.net.URISyntaxException;
import java.sql.SQLException;
import java.util.Enumeration;
import java.util.Observable;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.jdesktop.application.Action;
import org.jdesktop.application.ResourceMap;
import org.jdesktop.application.SingleFrameApplication;
import org.jdesktop.application.FrameView;
import org.jdesktop.application.TaskMonitor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FilenameFilter;
import java.net.URI;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Observer;
import java.util.Vector;
import javax.swing.Timer;
import javax.swing.Icon;
import javax.swing.JDialog;
import javax.swing.JFrame;
import oracle.jdbc.OracleResultSet;

/**
 * @brief The application's main frame.
 */
public class ZooView extends FrameView implements ActionListener, Observer {

	/// Application help URL.
	private static final String APP_HELP_URL =
			"http://www.stud.fit.vutbr.cz/~xzemek02/PDB/help/";

	/// How a length should be formatted.
	private static final DecimalFormat LENGTH_FORMAT = new DecimalFormat("0.00");

	/// How an area should be formatted.
	private static final DecimalFormat AREA_FORMAT = new DecimalFormat("0.00");

	/// Default map mode.
	private static final MapPanel.MapMode DEFAULT_MAP_MODE = MapPanel.MapMode.ANIMAL;

	/// Currently selected object data.
	private ObjectData _selectedObject = null;

	/**
	 * @brief JPGFilter.
	 */
	class JPGFilter implements FilenameFilter {

		public boolean accept(File dir, String name) {
			return (name.endsWith(".jpg"));
		}
	}

	/**
	 * @brief Constructor.
	 *
	 * @param[in] app Main application.
	 */
	public ZooView(SingleFrameApplication app) {
		super(app);

		initComponents();

		// status bar initialization - message timeout, idle icon and busy animation, etc
		ResourceMap resourceMap = getResourceMap();
		int messageTimeout = resourceMap.getInteger("StatusBar.messageTimeout");
		messageTimer = new Timer(messageTimeout, new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				statusMessageLabel.setText("");
			}
		});
		messageTimer.setRepeats(false);
		int busyAnimationRate = resourceMap.getInteger("StatusBar.busyAnimationRate");
		for (int i = 0; i < busyIcons.length; i++) {
			busyIcons[i] = resourceMap.getIcon("StatusBar.busyIcons[" + i + "]");
		}
		busyIconTimer = new Timer(busyAnimationRate, new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				busyIconIndex = (busyIconIndex + 1) % busyIcons.length;
				statusAnimationLabel.setIcon(busyIcons[busyIconIndex]);
			}
		});
		idleIcon = resourceMap.getIcon("StatusBar.idleIcon");
		statusAnimationLabel.setIcon(idleIcon);
		progressBar.setVisible(false);

		// connecting action tasks to status bar via TaskMonitor
		TaskMonitor taskMonitor = new TaskMonitor(getApplication().getContext());
		taskMonitor.addPropertyChangeListener(new java.beans.PropertyChangeListener() {

			public void propertyChange(java.beans.PropertyChangeEvent evt) {
				String propertyName = evt.getPropertyName();
				if ("started".equals(propertyName)) {
					if (!busyIconTimer.isRunning()) {
						statusAnimationLabel.setIcon(busyIcons[0]);
						busyIconIndex = 0;
						busyIconTimer.start();
					}
					progressBar.setVisible(true);
					progressBar.setIndeterminate(true);
				} else if ("done".equals(propertyName)) {
					busyIconTimer.stop();
					statusAnimationLabel.setIcon(idleIcon);
					progressBar.setVisible(false);
					progressBar.setValue(0);
				} else if ("message".equals(propertyName)) {
					String text = (String) (evt.getNewValue());
					statusMessageLabel.setText((text == null) ? "" : text);
					messageTimer.restart();
				} else if ("progress".equals(propertyName)) {
					int value = (Integer) (evt.getNewValue());
					progressBar.setVisible(true);
					progressBar.setIndeterminate(false);
					progressBar.setValue(value);
				}
			}
		});
		StatusMessageLabel.getInstance().init(statusMessageLabel);
		StatusMessageLabel.getInstance().setText("");

		// Do not log anything by default (user can change this by using
		// the "Toggle logger" menu item in "Help")
		ZooLogger.disableLogging();

		// Set default map mode
		map.setMapMode(DEFAULT_MAP_MODE);
	}

	@Action
	public void showAboutBox() {
		if (aboutBox == null) {
			JFrame mainFrame = ZooApp.getApplication().getMainFrame();
			aboutBox = new ZooAboutBox(mainFrame);
			aboutBox.setLocationRelativeTo(mainFrame);
		}
		aboutBox.setVisible(true);
	}

	/** This method is called from within the constructor to
	 * initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is
	 * always regenerated by the Form Editor.
	 */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        mainPanel = new javax.swing.JPanel();
        calendar = new zoo.CalendarPanel();
        map = new zoo.MapPanel();
        details = new zoo.DetailsPanel();
        navigation = new javax.swing.JPanel();
        upButton = new javax.swing.JButton();
        rightButton = new javax.swing.JButton();
        downButton = new javax.swing.JButton();
        leftButton = new javax.swing.JButton();
        zoomSlider = new javax.swing.JSlider();
        zoomfitButton = new javax.swing.JButton();
        mapsMode = new javax.swing.JPanel();
        mapMode = new javax.swing.JRadioButton();
        buildingMode = new javax.swing.JRadioButton();
        mapModeLabel = new javax.swing.JLabel();
        animalMode = new javax.swing.JRadioButton();
        menuBar = new javax.swing.JMenuBar();
        javax.swing.JMenu fileMenu = new javax.swing.JMenu();
        javax.swing.JMenuItem exitMenuItem = new javax.swing.JMenuItem();
        searchMenu = new javax.swing.JMenu();
        photoMenuItem = new javax.swing.JMenuItem();
        queriesMenu = new javax.swing.JMenu();
        queryPathLengthsMenuItem = new javax.swing.JMenuItem();
        queryExpAreaMenuItem = new javax.swing.JMenuItem();
        queryZooAreaMenuItem = new javax.swing.JMenuItem();
        queriesSeparator = new javax.swing.JSeparator();
        queryCountOfGeneraMenuItem = new javax.swing.JMenuItem();
        queryPavilionWithMostAnimalsMenuItem = new javax.swing.JMenuItem();
        dbMenu = new javax.swing.JMenu();
        dbMenuRestart = new javax.swing.JMenuItem();
        javax.swing.JMenu helpMenu = new javax.swing.JMenu();
        helpContentsMenuItem = new javax.swing.JMenuItem();
        jMenuItem1 = new javax.swing.JMenuItem();
        javax.swing.JMenuItem aboutMenuItem = new javax.swing.JMenuItem();
        statusPanel = new javax.swing.JPanel();
        javax.swing.JSeparator statusPanelSeparator = new javax.swing.JSeparator();
        statusMessageLabel = new javax.swing.JLabel();
        statusAnimationLabel = new javax.swing.JLabel();
        progressBar = new javax.swing.JProgressBar();
        mapModeButtonGroup = new javax.swing.ButtonGroup();

        mainPanel.setName("mainPanel"); // NOI18N

        calendar.setName("calendar"); // NOI18N

        map.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        map.setName("map"); // NOI18N
        map.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                mapMouseClicked(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                mapMouseExited(evt);
            }
            public void mousePressed(java.awt.event.MouseEvent evt) {
                mapMousePressed(evt);
            }
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                mapMouseReleased(evt);
            }
        });
        map.addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent evt) {
                mapComponentResized(evt);
            }
        });
        map.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseDragged(java.awt.event.MouseEvent evt) {
                mapMouseDragged(evt);
            }
            public void mouseMoved(java.awt.event.MouseEvent evt) {
                mapMouseMoved(evt);
            }
        });

        javax.swing.GroupLayout mapLayout = new javax.swing.GroupLayout(map);
        map.setLayout(mapLayout);
        mapLayout.setHorizontalGroup(
            mapLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 625, Short.MAX_VALUE)
        );
        mapLayout.setVerticalGroup(
            mapLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 670, Short.MAX_VALUE)
        );

        details.setName("details"); // NOI18N

        navigation.setName("navigation"); // NOI18N

        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(zoo.ZooApp.class).getContext().getResourceMap(ZooView.class);
        upButton.setIcon(resourceMap.getIcon("upButton.icon")); // NOI18N
        upButton.setText(resourceMap.getString("upButton.text")); // NOI18N
        upButton.setBorderPainted(false);
        upButton.setMaximumSize(new java.awt.Dimension(18, 19));
        upButton.setMinimumSize(new java.awt.Dimension(18, 19));
        upButton.setName("upButton"); // NOI18N
        upButton.setPreferredSize(new java.awt.Dimension(18, 19));
        upButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upButtonActionPerformed(evt);
            }
        });

        rightButton.setIcon(resourceMap.getIcon("rightButton.icon")); // NOI18N
        rightButton.setText(resourceMap.getString("rightButton.text")); // NOI18N
        rightButton.setBorderPainted(false);
        rightButton.setName("rightButton"); // NOI18N
        rightButton.setPreferredSize(new java.awt.Dimension(20, 18));
        rightButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightButtonActionPerformed(evt);
            }
        });

        downButton.setIcon(resourceMap.getIcon("downButton.icon")); // NOI18N
        downButton.setText(resourceMap.getString("downButton.text")); // NOI18N
        downButton.setBorderPainted(false);
        downButton.setName("downButton"); // NOI18N
        downButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downButtonActionPerformed(evt);
            }
        });

        leftButton.setIcon(resourceMap.getIcon("leftButton.icon")); // NOI18N
        leftButton.setText(resourceMap.getString("leftButton.text")); // NOI18N
        leftButton.setBorderPainted(false);
        leftButton.setName("leftButton"); // NOI18N
        leftButton.setPreferredSize(new java.awt.Dimension(19, 18));
        leftButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftButtonActionPerformed(evt);
            }
        });

        zoomSlider.setOrientation(javax.swing.JSlider.VERTICAL);
        zoomSlider.setValue(0);
        zoomSlider.setName("zoomSlider"); // NOI18N
        zoomSlider.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                zoomSliderMouseReleased(evt);
            }
        });
        zoomSlider.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                zoomSliderStateChanged(evt);
            }
        });

        zoomfitButton.setIcon(resourceMap.getIcon("zoomfitButton.icon")); // NOI18N
        zoomfitButton.setText(resourceMap.getString("zoomfitButton.text")); // NOI18N
        zoomfitButton.setBorderPainted(false);
        zoomfitButton.setMaximumSize(new java.awt.Dimension(20, 20));
        zoomfitButton.setMinimumSize(new java.awt.Dimension(20, 20));
        zoomfitButton.setName("zoomfitButton"); // NOI18N
        zoomfitButton.setPreferredSize(new java.awt.Dimension(20, 20));
        zoomfitButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                zoomfitButtonActionPerformed(evt);
            }
        });

        mapsMode.setName("mapsMode"); // NOI18N

        mapModeButtonGroup.add(mapMode);
        mapMode.setText(resourceMap.getString("mapMode.text")); // NOI18N
        mapMode.setName("mapMode"); // NOI18N
        mapMode.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                mapModeActionPerformed(evt);
            }
        });

        mapModeButtonGroup.add(buildingMode);
        buildingMode.setText(resourceMap.getString("buildingMode.text")); // NOI18N
        buildingMode.setName("buildingMode"); // NOI18N
        buildingMode.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buildingModeActionPerformed(evt);
            }
        });

        mapModeLabel.setText(resourceMap.getString("mapModeLabel.text")); // NOI18N
        mapModeLabel.setName("mapModeLabel"); // NOI18N

        mapModeButtonGroup.add(animalMode);
        animalMode.setSelected(true);
        animalMode.setText(resourceMap.getString("animalMode.text")); // NOI18N
        animalMode.setName("animalMode"); // NOI18N
        animalMode.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                animalModeActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout mapsModeLayout = new javax.swing.GroupLayout(mapsMode);
        mapsMode.setLayout(mapsModeLayout);
        mapsModeLayout.setHorizontalGroup(
            mapsModeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(mapsModeLayout.createSequentialGroup()
                .addGroup(mapsModeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(mapsModeLayout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(mapModeLabel))
                    .addComponent(mapMode)
                    .addComponent(buildingMode)
                    .addComponent(animalMode))
                .addContainerGap(16, Short.MAX_VALUE))
        );
        mapsModeLayout.setVerticalGroup(
            mapsModeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(mapsModeLayout.createSequentialGroup()
                .addComponent(mapModeLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(mapMode)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(buildingMode)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(animalMode))
        );

        javax.swing.GroupLayout navigationLayout = new javax.swing.GroupLayout(navigation);
        navigation.setLayout(navigationLayout);
        navigationLayout.setHorizontalGroup(
            navigationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(navigationLayout.createSequentialGroup()
                .addGap(6, 6, 6)
                .addGroup(navigationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(navigationLayout.createSequentialGroup()
                        .addComponent(leftButton, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(navigationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(upButton, javax.swing.GroupLayout.PREFERRED_SIZE, 18, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(zoomfitButton, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(downButton, javax.swing.GroupLayout.PREFERRED_SIZE, 18, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(rightButton, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(mapsMode, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(zoomSlider, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        navigationLayout.setVerticalGroup(
            navigationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(navigationLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(upButton, javax.swing.GroupLayout.PREFERRED_SIZE, 19, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(navigationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(leftButton, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(navigationLayout.createSequentialGroup()
                        .addGroup(navigationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(rightButton, javax.swing.GroupLayout.PREFERRED_SIZE, 18, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(zoomfitButton, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(downButton, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(mapsMode, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
            .addComponent(zoomSlider, javax.swing.GroupLayout.DEFAULT_SIZE, 251, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout mainPanelLayout = new javax.swing.GroupLayout(mainPanel);
        mainPanel.setLayout(mainPanelLayout);
        mainPanelLayout.setHorizontalGroup(
            mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(mainPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(map, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(navigation, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(calendar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(details, javax.swing.GroupLayout.PREFERRED_SIZE, 218, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );
        mainPanelLayout.setVerticalGroup(
            mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(mainPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(mainPanelLayout.createSequentialGroup()
                        .addComponent(navigation, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(calendar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(details, javax.swing.GroupLayout.PREFERRED_SIZE, 237, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addContainerGap())
                    .addComponent(map, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
        );

        menuBar.setName("menuBar"); // NOI18N

        fileMenu.setText(resourceMap.getString("fileMenu.text")); // NOI18N
        fileMenu.setName("fileMenu"); // NOI18N

        javax.swing.ActionMap actionMap = org.jdesktop.application.Application.getInstance(zoo.ZooApp.class).getContext().getActionMap(ZooView.class, this);
        exitMenuItem.setAction(actionMap.get("quit")); // NOI18N
        exitMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_X, java.awt.event.InputEvent.CTRL_MASK));
        exitMenuItem.setName("exitMenuItem"); // NOI18N
        fileMenu.add(exitMenuItem);

        menuBar.add(fileMenu);

        searchMenu.setText(resourceMap.getString("searchMenu.text")); // NOI18N
        searchMenu.setName("searchMenu"); // NOI18N

        photoMenuItem.setAction(actionMap.get("searchPhotoClicked")); // NOI18N
        photoMenuItem.setText(resourceMap.getString("photoMenuItem.text")); // NOI18N
        photoMenuItem.setName("photoMenuItem"); // NOI18N
        searchMenu.add(photoMenuItem);

        menuBar.add(searchMenu);

        queriesMenu.setText(resourceMap.getString("queriesMenu.text")); // NOI18N
        queriesMenu.setName("queriesMenu"); // NOI18N

        queryPathLengthsMenuItem.setText(resourceMap.getString("queryPathLengthsMenuItem.text")); // NOI18N
        queryPathLengthsMenuItem.setName("queryPathLengthsMenuItem"); // NOI18N
        queryPathLengthsMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                queryPathLengthsMenuItemActionPerformed(evt);
            }
        });
        queriesMenu.add(queryPathLengthsMenuItem);

        queryExpAreaMenuItem.setText(resourceMap.getString("queryExpAreaMenuItem.text")); // NOI18N
        queryExpAreaMenuItem.setName("queryExpAreaMenuItem"); // NOI18N
        queryExpAreaMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                queryExpAreaMenuItemActionPerformed(evt);
            }
        });
        queriesMenu.add(queryExpAreaMenuItem);

        queryZooAreaMenuItem.setText(resourceMap.getString("queryZooAreaMenuItem.text")); // NOI18N
        queryZooAreaMenuItem.setName("queryZooAreaMenuItem"); // NOI18N
        queryZooAreaMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                queryZooAreaMenuItemActionPerformed(evt);
            }
        });
        queriesMenu.add(queryZooAreaMenuItem);

        queriesSeparator.setName("queriesSeparator"); // NOI18N
        queriesMenu.add(queriesSeparator);

        queryCountOfGeneraMenuItem.setText(resourceMap.getString("queryCountOfGeneraMenuItem.text")); // NOI18N
        queryCountOfGeneraMenuItem.setName("queryCountOfGeneraMenuItem"); // NOI18N
        queryCountOfGeneraMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                queryCountOfGeneraMenuItemActionPerformed(evt);
            }
        });
        queriesMenu.add(queryCountOfGeneraMenuItem);

        queryPavilionWithMostAnimalsMenuItem.setText(resourceMap.getString("queryPavilionWithMostAnimalsMenuItem.text")); // NOI18N
        queryPavilionWithMostAnimalsMenuItem.setName("queryPavilionWithMostAnimalsMenuItem"); // NOI18N
        queryPavilionWithMostAnimalsMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                queryPavilionWithMostAnimalsMenuItemActionPerformed(evt);
            }
        });
        queriesMenu.add(queryPavilionWithMostAnimalsMenuItem);

        menuBar.add(queriesMenu);

        dbMenu.setText(resourceMap.getString("dbMenu.text")); // NOI18N
        dbMenu.setName("dbMenu"); // NOI18N

        dbMenuRestart.setText(resourceMap.getString("dbMenuRestart.text")); // NOI18N
        dbMenuRestart.setName("dbMenuRestart"); // NOI18N
        dbMenuRestart.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                dbMenuRestartActionPerformed(evt);
            }
        });
        dbMenu.add(dbMenuRestart);

        menuBar.add(dbMenu);

        helpMenu.setText(resourceMap.getString("helpMenu.text")); // NOI18N
        helpMenu.setName("helpMenu"); // NOI18N

        helpContentsMenuItem.setAction(actionMap.get("showHelpContents")); // NOI18N
        helpContentsMenuItem.setText(resourceMap.getString("helpContentsMenuItem.text")); // NOI18N
        helpContentsMenuItem.setName("helpContentsMenuItem"); // NOI18N
        helpMenu.add(helpContentsMenuItem);

        jMenuItem1.setAction(actionMap.get("toggleLogger")); // NOI18N
        jMenuItem1.setText(resourceMap.getString("jMenuItem1.text")); // NOI18N
        jMenuItem1.setName("jMenuItem1"); // NOI18N
        helpMenu.add(jMenuItem1);

        aboutMenuItem.setAction(actionMap.get("showAboutBox")); // NOI18N
        aboutMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_A, java.awt.event.InputEvent.ALT_MASK));
        aboutMenuItem.setName("aboutMenuItem"); // NOI18N
        helpMenu.add(aboutMenuItem);

        menuBar.add(helpMenu);

        statusPanel.setName("statusPanel"); // NOI18N

        statusPanelSeparator.setName("statusPanelSeparator"); // NOI18N

        statusMessageLabel.setText(resourceMap.getString("statusMessageLabel.text")); // NOI18N
        statusMessageLabel.setName("statusMessageLabel"); // NOI18N

        statusAnimationLabel.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        statusAnimationLabel.setName("statusAnimationLabel"); // NOI18N

        progressBar.setName("progressBar"); // NOI18N

        javax.swing.GroupLayout statusPanelLayout = new javax.swing.GroupLayout(statusPanel);
        statusPanel.setLayout(statusPanelLayout);
        statusPanelLayout.setHorizontalGroup(
            statusPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(statusPanelSeparator, javax.swing.GroupLayout.DEFAULT_SIZE, 899, Short.MAX_VALUE)
            .addGroup(statusPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(statusMessageLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 615, Short.MAX_VALUE)
                .addComponent(progressBar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(statusAnimationLabel)
                .addContainerGap())
        );
        statusPanelLayout.setVerticalGroup(
            statusPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(statusPanelLayout.createSequentialGroup()
                .addComponent(statusPanelSeparator, javax.swing.GroupLayout.PREFERRED_SIZE, 2, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(statusPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(statusMessageLabel)
                    .addComponent(statusAnimationLabel)
                    .addComponent(progressBar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(3, 3, 3))
        );

        setComponent(mainPanel);
        setMenuBar(menuBar);
        setStatusBar(statusPanel);
    }// </editor-fold>//GEN-END:initComponents
    private void mapComponentResized(java.awt.event.ComponentEvent evt) {//GEN-FIRST:event_mapComponentResized
		redrawPanels();

		// Notify me when the date is changed
		Calendar.getInstance().addObserver(this);
    }//GEN-LAST:event_mapComponentResized

    private void mapMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_mapMouseClicked
		Point clickPos = evt.getPoint();

		// Was is a left mouse button click?
		if ((evt.getModifiers() & MouseEvent.BUTTON1_MASK) == MouseEvent.BUTTON1_MASK) {
			switch (map.getMouseMode()) {
				case DEFAULT:
					// Transform map coordinates into db coordinate.
					Point dbClickPos = View.getInstance().map2db(clickPos);

					// Get the nearest object that was selected by this click
					setSelectedObject(ObjectOperations.getObjectOnPosition(
									dbClickPos, calendar.getDate()));

					if (_selectedObject != null) {
						StatusMessageLabel.getInstance().setText("You have selected: " + _selectedObject.name);
					}

					break;

				case MOVING:
					View.getInstance().moveMouse();
					redrawPanels();
					break;

				case INSERTING:
					map.mouseClickedInsertAdd(clickPos);
					break;

				case DRAGGING:
					// not used here (see DragAndDrop class)
					break;

				default:
					Logger.getLogger("zoo").log(Level.WARNING,
						"Invalid mouse mode in switch.");
					break;
			}
		}

		// Was it a right mouse button click?
		if ((evt.getModifiers() & MouseEvent.BUTTON3_MASK) == MouseEvent.BUTTON3_MASK) {
			switch (map.getMouseMode()) {
				case INSERTING:
					map.mouseClickedInsertLast(clickPos);
					break;
				default:
					// Transform map coordinates into db coordinate.
					Point dbClickPos = View.getInstance().map2db(clickPos);

					// Get the nearest object that was selected by this click
					setSelectedObject(ObjectOperations.getObjectOnPosition(
									dbClickPos, calendar.getDate()));
					PopupMenu.DisplayMenu(evt, this, calendar.getDate(),
									ObjectOperations.getObjectOnPosition(dbClickPos, calendar.getDate()));
					break;
			}
		}
	}//GEN-LAST:event_mapMouseClicked

	/**
	 * @brief Zoom in map.
	 */
    private void rightButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightButtonActionPerformed
			View.getInstance().moveRight();
			redrawPanels();
    }//GEN-LAST:event_rightButtonActionPerformed

    private void leftButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftButtonActionPerformed
			View.getInstance().moveLeft();
			redrawPanels();
    }//GEN-LAST:event_leftButtonActionPerformed

    private void downButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downButtonActionPerformed
			View.getInstance().moveDown();
			redrawPanels();
    }//GEN-LAST:event_downButtonActionPerformed

    private void upButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upButtonActionPerformed
			View.getInstance().moveUp();
			redrawPanels();
    }//GEN-LAST:event_upButtonActionPerformed

    private void zoomSliderStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_zoomSliderStateChanged

    }//GEN-LAST:event_zoomSliderStateChanged

    private void zoomSliderMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_zoomSliderMouseReleased
			View.getInstance().slider2zoom(zoomSlider.getValue());
			redrawPanels();
    }//GEN-LAST:event_zoomSliderMouseReleased

    private void zoomfitButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_zoomfitButtonActionPerformed
			View.getInstance().zoomFit(map.getWidth(), map.getHeight());
			zoomSlider.setValue(View.getInstance().zoom2slider());
			redrawPanels();
    }//GEN-LAST:event_zoomfitButtonActionPerformed

    private void mapMouseMoved(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_mapMouseMoved
			final int BORDER = 20;

			Point mousePoint = evt.getPoint();


			switch (map.getMouseMode()) {
				case INSERTING:
					map.mouseMovedInsert(mousePoint);
					break;
				default:
			if (mousePoint.x < BORDER) {
				View.getInstance().setMouseDirection(View.Direction.LEFT);
				map.setMouseMode(MapPanel.MouseMode.MOVING);
				map.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
				StatusMessageLabel.getInstance().setText("Moving left");
			} else if (mousePoint.x > (map.getWidth() - BORDER)) {
				View.getInstance().setMouseDirection(View.Direction.RIGHT);
				map.setMouseMode(MapPanel.MouseMode.MOVING);
				map.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
				StatusMessageLabel.getInstance().setText("Moving right");
			} else if (mousePoint.y < BORDER) {
				View.getInstance().setMouseDirection(View.Direction.UP);
				map.setMouseMode(MapPanel.MouseMode.MOVING);
				map.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
				StatusMessageLabel.getInstance().setText("Moving up");
			} else if (mousePoint.y > (map.getHeight() - BORDER)) {
				View.getInstance().setMouseDirection(View.Direction.DOWN);
				map.setMouseMode(MapPanel.MouseMode.MOVING);
				map.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
				StatusMessageLabel.getInstance().setText("Moving down");
			} else {
				if (map.getMouseMode() == MapPanel.MouseMode.MOVING) {
					map.setMouseMode(MapPanel.MouseMode.DEFAULT);
					map.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
					StatusMessageLabel.getInstance().setText("");
				}
			}
					break;
			}
    }//GEN-LAST:event_mapMouseMoved

    private void dbMenuRestartActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_dbMenuRestartActionPerformed
			ArrayList<String> imgsDirs = new ArrayList<String>();
			imgsDirs.add("pavilion");
			imgsDirs.add("refreshment");
			imgsDirs.add("exhibit");
			imgsDirs.add("animal");

			// 1. run SQL script
			String sqlScriptPath = System.getProperty("user.dir") + "/db/sql/data.sql";

			DBConnection.getInstance().runSqlScript(sqlScriptPath);

			// 2. upload images
			String imgsDir = System.getProperty("user.dir") + "/db/imgs/";

			for (Iterator<String> it = imgsDirs.iterator(); it.hasNext();) {
				int numberOfFiles = 0;
				String tableName = it.next();
				String path = imgsDir + tableName + "/";

				File dir = new File(path);
				numberOfFiles = dir.listFiles(new JPGFilter()).length;

				for (int i = 1; i <= numberOfFiles; i++) {
					try {
						String imgPath = path + tableName + i + ".jpg";
						PhotoOperations.uploadPhotoToDb(imgPath, tableName, i);
					} catch (SQLException ex) {
						Logger.getLogger("zoo").log(Level.SEVERE,
								"Exception caught (" + ex.getClass() + "): " + ex.getMessage());
						Dialogs.displayErrorMessage(null,
								"There was a database error during the last operation.",
								"Database error");
					}
				}
			}

			redrawPanels();
    }//GEN-LAST:event_dbMenuRestartActionPerformed

    private void mapModeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_mapModeActionPerformed
			map.setMapMode(MapPanel.MapMode.MAP);
			redrawPanels();
}//GEN-LAST:event_mapModeActionPerformed

    private void buildingModeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buildingModeActionPerformed
			map.setMapMode(MapPanel.MapMode.BUILDING);
			redrawPanels();
}//GEN-LAST:event_buildingModeActionPerformed

    private void animalModeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_animalModeActionPerformed
			map.setMapMode(MapPanel.MapMode.ANIMAL);
			redrawPanels();
}//GEN-LAST:event_animalModeActionPerformed

    private void mapMouseDragged(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_mapMouseDragged
			map.setMouseMode(MapPanel.MouseMode.DRAGGING);
			statusMessageLabel.setText("dragging");
			map.mouseDragged(evt.getPoint());
    }//GEN-LAST:event_mapMouseDragged

    private void mapMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_mapMousePressed
			map.mousePressed(evt.getPoint());
    }//GEN-LAST:event_mapMousePressed

    private void mapMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_mapMouseReleased
			if (map.getMouseMode() != MapPanel.MouseMode.INSERTING) {
				map.setMouseMode(MapPanel.MouseMode.DEFAULT);
				StatusMessageLabel.getInstance().setText("");
				map.mouseReleased(evt.getPoint(), this);
			}
    }//GEN-LAST:event_mapMouseReleased

	private void queryPathLengthsMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_queryPathLengthsMenuItemActionPerformed
		try {
			double allPathsLength = TemporalQuery.getLengthOfAllPaths(
					Calendar.getInstance().getDate());
			Dialogs.displayInformationMessage(null,
					"The length of all paths is: " + LENGTH_FORMAT.format(
						allPathsLength) + " m.\n",
					"Query result");
		} catch (SQLException ex) {//GEN-LAST:event_queryPathLengthsMenuItemActionPerformed
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			Dialogs.displayErrorMessage(null,
					"There was a database error during the last operation.",
					"Database error");
		}
	}

	private void queryExpAreaMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_queryExpAreaMenuItemActionPerformed
		try {
			double allExpositionsArea = TemporalQuery.getAreaOfAllExpositions(
					Calendar.getInstance().getDate());
			Dialogs.displayInformationMessage(null,
					"The area of all expositions is: " + AREA_FORMAT.format(
						allExpositionsArea) + " m^2.\n",
					"Query result");
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			Dialogs.displayErrorMessage(null,
					"There was a database error during the last operation.",
					"Database error");
		}
	}//GEN-LAST:event_queryExpAreaMenuItemActionPerformed

	private void queryZooAreaMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_queryZooAreaMenuItemActionPerformed
		try {
			double zooArea = TemporalQuery.getZooArea(
					Calendar.getInstance().getDate());
			Dialogs.displayInformationMessage(null,
					"The total area of the ZOO is: " + AREA_FORMAT.format(
						zooArea) + " m^2.\n",
					"Query result");
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			Dialogs.displayErrorMessage(null,
					"There was a database error during the last operation.",
					"Database error");
		}
	}//GEN-LAST:event_queryZooAreaMenuItemActionPerformed

	private void queryCountOfGeneraMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_queryCountOfGeneraMenuItemActionPerformed
		try {
			int count = TemporalQuery.getCountOfGenera(
					Calendar.getInstance().getDate());
			Dialogs.displayInformationMessage("Genera: " + count,
					"Count of genera in the ZOO");
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			Dialogs.displayErrorMessage(null,
					"There was a database error during the last operation.",
					"Database error");
		}
	}//GEN-LAST:event_queryCountOfGeneraMenuItemActionPerformed

	private void queryPavilionWithMostAnimalsMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_queryPavilionWithMostAnimalsMenuItemActionPerformed
		try {
			String pavilion = TemporalQuery.getPavilionWithMostAnimals(
					Calendar.getInstance().getDate());
			Dialogs.displayInformationMessage(pavilion, "Pavilion with most animals");
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, null, ex);
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			Dialogs.displayErrorMessage(null,
					"There was a database error during the last operation.",
					"Database error");
		}
	}//GEN-LAST:event_queryPavilionWithMostAnimalsMenuItemActionPerformed

	private void mapMouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_mapMouseExited
		map.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
		StatusMessageLabel.getInstance().setText("");
	}//GEN-LAST:event_mapMouseExited

	public void setMouseMode(MapPanel.MouseMode m) {
		map.setMouseMode(m);
	}

	public void setStatusMessageLabel(String txt) {
		statusMessageLabel.setText(txt);
	}

	/**
	 * @brief Redraws all panels.
	 */
	public void redrawPanels() {
		map.startDrawing(calendar.getDate());
		drawZooMap();

		if (_selectedObject != null) {
			map.fillObject(_selectedObject);

			details.displayObjectInfo(_selectedObject);
		}
		else {
			details.displayObjectInfo(new ObjectData());
		}

		map.saveBackBuffer();
		map.endDrawing();
	}

	/**
	 * Selects new object and redraws panels.
	 *
	 * @param selected  New object to be selected.
	 */
	public void setSelectedObject(ObjectData selected) {
		_selectedObject = selected;
		redrawPanels();
	}

	/**
	 * @brief Draws ZOO map.
	 *
	 * Note: map.startDrawing() must be called before this method.
	 */
	private void drawZooMap() {
		try {
			Enumeration objectsEnum = Objects.getObjectTableNames();
			while (objectsEnum.hasMoreElements()) {
				String tableName = (String) objectsEnum.nextElement();

				// Get all row IDs
				// There is a problem when I run another query when I'm already
				// iterating over some result set which results into the first
				// result set getting empty...
				// So I have to first get all IDs and then work with them.
				Vector rowIds = new Vector();
				String query = "SELECT row_id FROM " + tableName +
								" WHERE " + TemporalQuery.toTemporalAtDate(calendar.getDate());
				OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(query);
				while (rs.next()) {
					rowIds.add(rs.getInt("row_id"));
				}

				// Disaplay all objects
				Enumeration objectIdsEnum = rowIds.elements();
				while (objectIdsEnum.hasMoreElements()) {
					ObjectData objectData = Objects.getObjectData(tableName,
									((Integer) objectIdsEnum.nextElement()).intValue());
					map.drawObject(objectData);
				}
			}
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}
	}

	@Action
	public void searchPhotoClicked() {
		// Display photo search window
		PhotoSearchWindow photoSearchWindow = new PhotoSearchWindow();
		photoSearchWindow.setLocationRelativeTo(getFrame());
		photoSearchWindow.setVisible(true);
	}

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JRadioButton animalMode;
    private javax.swing.JRadioButton buildingMode;
    private zoo.CalendarPanel calendar;
    private javax.swing.JMenu dbMenu;
    private javax.swing.JMenuItem dbMenuRestart;
    private zoo.DetailsPanel details;
    private javax.swing.JButton downButton;
    private javax.swing.JMenuItem helpContentsMenuItem;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JButton leftButton;
    private javax.swing.JPanel mainPanel;
    private zoo.MapPanel map;
    private javax.swing.JRadioButton mapMode;
    private javax.swing.ButtonGroup mapModeButtonGroup;
    private javax.swing.JLabel mapModeLabel;
    private javax.swing.JPanel mapsMode;
    private javax.swing.JMenuBar menuBar;
    private javax.swing.JPanel navigation;
    private javax.swing.JMenuItem photoMenuItem;
    private javax.swing.JProgressBar progressBar;
    private javax.swing.JMenu queriesMenu;
    private javax.swing.JSeparator queriesSeparator;
    private javax.swing.JMenuItem queryCountOfGeneraMenuItem;
    private javax.swing.JMenuItem queryExpAreaMenuItem;
    private javax.swing.JMenuItem queryPathLengthsMenuItem;
    private javax.swing.JMenuItem queryPavilionWithMostAnimalsMenuItem;
    private javax.swing.JMenuItem queryZooAreaMenuItem;
    private javax.swing.JButton rightButton;
    private javax.swing.JMenu searchMenu;
    private javax.swing.JLabel statusAnimationLabel;
    private javax.swing.JLabel statusMessageLabel;
    private javax.swing.JPanel statusPanel;
    private javax.swing.JButton upButton;
    private javax.swing.JSlider zoomSlider;
    private javax.swing.JButton zoomfitButton;
    // End of variables declaration//GEN-END:variables
	private final Timer messageTimer;
	private final Timer busyIconTimer;
	private final Icon idleIcon;
	private final Icon[] busyIcons = new Icon[15];
	private int busyIconIndex = 0;
	private JDialog aboutBox;

	public void actionPerformed(ActionEvent arg0) {
		PopupMenu.PopupMenuClicked(arg0, _selectedObject, this);
	}

	public void update(Observable arg0, Object arg1) {
		setSelectedObject(null);
	}

	@Action
	public void showHelpContents() {
		try {
			java.awt.Desktop.getDesktop().browse(new URI(APP_HELP_URL));
		} catch (URISyntaxException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		} catch (IOException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}
	}

	@Action
	public void toggleLogger() {
		if (ZooLogger.isLoggingEnabled()) {
			// Logging is enabled -> disable it
			ZooLogger.disableLogging();
		} else {
			// Logging is disabled -> enable it
			ZooLogger.enableLogging();
		}
	}
}
