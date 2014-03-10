/*
 * Project: RLJC - Really Lightweight Jabber Client
 * Author:  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * 
 * Copyright (C) 2009 Petr Zemek
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package rljc;

import java.awt.AWTException;
import java.awt.Dimension;
import java.awt.MenuItem;
import java.awt.Point;
import java.awt.PopupMenu;
import java.awt.SystemTray;
import java.awt.TrayIcon;
import java.awt.event.MouseEvent;
import org.jdesktop.application.Action;
import org.jdesktop.application.ResourceMap;
import org.jdesktop.application.SingleFrameApplication;
import org.jdesktop.application.FrameView;
import org.jdesktop.application.TaskMonitor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.event.WindowAdapter;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import javax.swing.Timer;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeSelectionModel;
import org.jivesoftware.smack.ConnectionConfiguration;
import org.jivesoftware.smack.PacketListener;
import org.jivesoftware.smack.Roster;
import org.jivesoftware.smack.RosterEntry;
import org.jivesoftware.smack.SASLAuthentication;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.filter.AndFilter;
import org.jivesoftware.smack.filter.PacketFilter;
import org.jivesoftware.smack.filter.PacketTypeFilter;
import org.jivesoftware.smack.filter.ToContainsFilter;
import org.jivesoftware.smack.packet.Message;
import org.jivesoftware.smack.packet.Packet;

/**
 * The main frame of the application.
 */
public class View extends FrameView implements MouseListener, IChatPanelHandler,
		PacketListener {

	/** Path to the application directory in the user's home directory. */
	private static final String APP_DIR_PATH = System.getProperty("user.home") +
			File.separator + ".rljc";
	/** Name of the configuration file. */
	private static final String CONFIG_FILE_NAME = "configuration";
	/** Minimal number in the progress bar. */
	private static final int PROGRESS_BAR_MIN = 0;
	/** Maximal number in the progress bar. */
	private static final int PROGRESS_BAR_MAX = 100;
	
	/** Application configuration. */
	private IConfig appConfig = null;
	
	/** Connection to the Jabber server. */
	private XMPPConnection conn = null;
	
	/** Currently active chats. */
	private List<ChatPanel> activeChatHandlerPanels = new ArrayList<ChatPanel>();
	/** Currently active single chat windows. */
	private List<SingleWindowChat> activeSingleChatWindows = new ArrayList<SingleWindowChat>();
	/** Currently active multiple chat windows. */
	private List<MultipleWindowChat> activeMultipleChatWindows = new ArrayList<MultipleWindowChat>();
	
	/**
	 * Constructs a View object.
	 * 
	 * @param app Application.
	 */
    public View(SingleFrameApplication app) {
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
                    String text = (String)(evt.getNewValue());
                    statusMessageLabel.setText((text == null) ? "" : text);
                    messageTimer.restart();
                } else if ("progress".equals(propertyName)) {
                    int value = (Integer)(evt.getNewValue());
                    progressBar.setVisible(true);
                    progressBar.setIndeterminate(false);
                    progressBar.setValue(value);
                }
            }
        });
		
		// Check whether the user has an application directory. If not, create one.
		File appDir = new File(APP_DIR_PATH);
		if (!appDir.exists()) {
			if (!appDir.mkdir()) {
				Dialogs.displayErrorMessage("Cannot create application configuration directory:\n" +
						APP_DIR_PATH, "Error");
			}
		}
		
		// Initialize the configuration
		String appConfigFilePath = APP_DIR_PATH + File.separator + CONFIG_FILE_NAME;
		File appConfigFile = new File(appConfigFilePath);
		try {
			if (!appConfigFile.exists()) {
				// Create a new configuration file
				appConfigFile.createNewFile();
			}
			
			// Read the configuration
			appConfig = FileConfig.fromFile(appConfigFile);
			if (appConfig == null) {
				Dialogs.displayErrorMessage("Some value in the confiugration file (" +
					appConfigFilePath + ") is invalid", "Error");
			}
		} catch (IOException ex) {
			Dialogs.displayErrorMessage("Cannot read configuration file:\n" +
					appConfigFilePath, "Error");
		}
		
		final JFrame mainFrame = getFrame();
		// Set proper minimal frame size
		mainFrame.setMinimumSize(new Dimension(150, 200));
		
		// Make sure that the application config is saved when the main application
		// window gets closed
		mainFrame.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(java.awt.event.WindowEvent e) {
				// Store window dimensions and position
				Dimension size = mainFrame.getSize();
				Point location = mainFrame.getLocationOnScreen();
				appConfig.setAppWidth(size.width);
				appConfig.setAppHeight(size.height);
				appConfig.setAppXPos(location.x);
				appConfig.setAppYPos(location.y);
				
				appConfig.saveConfiguration();
				
				disconnectFromServer();
			}
		});
				
		// Set menu items
		connectItem.setEnabled(true);
		disconnectItem.setEnabled(false);
		multipleChatsItem.setState(appConfig.getMultipleChats());
		
		// Set progrress bar
		progressBar.setMinimum(PROGRESS_BAR_MIN);
		progressBar.setMaximum(PROGRESS_BAR_MAX);
		progressBar.setStringPainted(true);
		
		setApplicationIcon();
		setSystemTrayIcon();
		generateRoster();
    }

	/**
	 * Resizes and repositions the main application window according
	 * to the settings in the configuration.
	 */
	public void resizeAndRepos() {
		// Location
		int appXPos = appConfig.getAppXPos();
		int appYPos = appConfig.getAppYPos();
		if (appXPos > 0 && appYPos > 0) {
			getFrame().setLocation(appXPos, appYPos);
		}
		
		// Size
		int appWidth = appConfig.getAppWidth();
		int appHeight = appConfig.getAppHeight();
		if (appWidth > 0 && appHeight > 0) {
			getFrame().setSize(appWidth, appHeight);
		}
	}
	
	/**
	 * Creates and sets the application icon.
	 */
	public void setApplicationIcon() {
		ImageIcon imageIcon = new ImageIcon("src/resources/app-icon.png");
		getFrame().setIconImage(imageIcon.getImage());
	}
	
	/**
	 * Creates and sets a system tray icon.
	 */
	public void setSystemTrayIcon() {
		try {
			if (!SystemTray.isSupported()) {
				return;
			}

			// Create a popup menu
			PopupMenu menu = new PopupMenu();
			menu.add(new MenuItem("Quit"));
			menu.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent ev) {
					if ("Quit".equals(ev.getActionCommand())) {
						System.exit(0);
					}
				}
			});

			// Create an icon
			ImageIcon imageIcon = new ImageIcon("src/resources/tray-icon.png");
			TrayIcon trayIcon = new TrayIcon(imageIcon.getImage(), "RLJC", menu);

			// Add the icon to the tray
			SystemTray tray = SystemTray.getSystemTray();
			tray.add(trayIcon);
		} catch (AWTException ex) {
			Logger.getLogger().info("System tray error: " + ex);
		}
	}
	
	/**
	 * Shows the about box.
	 */
    @Action
    public void showAboutBox() {
		if (aboutBox == null) {
			JFrame mainFrame = App.getApplication().getMainFrame();
			aboutBox = new AboutBox(mainFrame);
			aboutBox.setLocationRelativeTo(mainFrame);
		}
		aboutBox.setVisible(true);
    }

	/**
	 * Shows the configuration window.
	 */
	@Action
	public void showConfigWindow() {
		ConfigWindow cw = new ConfigWindow(appConfig);
		cw.setLocationRelativeTo(null); // Center the window
		cw.setVisible(true);
	}
	
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        mainPanel = new javax.swing.JPanel();
        scrollPane = new javax.swing.JScrollPane();
        roster = new javax.swing.JTree();
        menuBar = new javax.swing.JMenuBar();
        javax.swing.JMenu fileMenu = new javax.swing.JMenu();
        connectItem = new javax.swing.JMenuItem();
        disconnectItem = new javax.swing.JMenuItem();
        javax.swing.JMenuItem exitItem = new javax.swing.JMenuItem();
        settingsMenu = new javax.swing.JMenu();
        multipleChatsItem = new javax.swing.JCheckBoxMenuItem();
        configureItem = new javax.swing.JMenuItem();
        javax.swing.JMenu helpMenu = new javax.swing.JMenu();
        javax.swing.JMenuItem aboutBoxItem = new javax.swing.JMenuItem();
        statusPanel = new javax.swing.JPanel();
        javax.swing.JSeparator statusPanelSeparator = new javax.swing.JSeparator();
        statusMessageLabel = new javax.swing.JLabel();
        statusAnimationLabel = new javax.swing.JLabel();
        progressBar = new javax.swing.JProgressBar();

        mainPanel.setName("mainPanel"); // NOI18N
        mainPanel.setLayout(new javax.swing.BoxLayout(mainPanel, javax.swing.BoxLayout.LINE_AXIS));

        scrollPane.setName("scrollPane"); // NOI18N

        roster.setName("roster"); // NOI18N
        scrollPane.setViewportView(roster);

        mainPanel.add(scrollPane);

        menuBar.setName("menuBar"); // NOI18N

        javax.swing.ActionMap actionMap = org.jdesktop.application.Application.getInstance(rljc.App.class).getContext().getActionMap(View.class, this);
        fileMenu.setAction(actionMap.get("connectToServer")); // NOI18N
        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(rljc.App.class).getContext().getResourceMap(View.class);
        fileMenu.setText(resourceMap.getString("fileMenu.text")); // NOI18N
        fileMenu.setName("fileMenu"); // NOI18N

        connectItem.setAction(actionMap.get("connectToServer")); // NOI18N
        connectItem.setText(resourceMap.getString("connectItem.text")); // NOI18N
        connectItem.setName("connectItem"); // NOI18N
        fileMenu.add(connectItem);

        disconnectItem.setAction(actionMap.get("disconnectFromServer")); // NOI18N
        disconnectItem.setText(resourceMap.getString("disconnectItem.text")); // NOI18N
        disconnectItem.setName("disconnectItem"); // NOI18N
        fileMenu.add(disconnectItem);

        exitItem.setAction(actionMap.get("quit")); // NOI18N
        exitItem.setName("exitItem"); // NOI18N
        fileMenu.add(exitItem);

        menuBar.add(fileMenu);

        settingsMenu.setText(resourceMap.getString("settingsMenu.text")); // NOI18N
        settingsMenu.setName("settingsMenu"); // NOI18N

        multipleChatsItem.setAction(actionMap.get("toggleMultipleChatsInOneWindow")); // NOI18N
        multipleChatsItem.setSelected(true);
        multipleChatsItem.setText(resourceMap.getString("multipleChatsItem.text")); // NOI18N
        multipleChatsItem.setName("multipleChatsItem"); // NOI18N
        settingsMenu.add(multipleChatsItem);

        configureItem.setAction(actionMap.get("showConfigWindow")); // NOI18N
        configureItem.setText(resourceMap.getString("configureItem.text")); // NOI18N
        configureItem.setName("configureItem"); // NOI18N
        settingsMenu.add(configureItem);

        menuBar.add(settingsMenu);

        helpMenu.setText(resourceMap.getString("helpMenu.text")); // NOI18N
        helpMenu.setName("helpMenu"); // NOI18N

        aboutBoxItem.setAction(actionMap.get("showAboutBox")); // NOI18N
        aboutBoxItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_A, java.awt.event.InputEvent.CTRL_MASK));
        aboutBoxItem.setName("aboutBoxItem"); // NOI18N
        helpMenu.add(aboutBoxItem);

        menuBar.add(helpMenu);

        statusPanel.setName("statusPanel"); // NOI18N

        statusPanelSeparator.setName("statusPanelSeparator"); // NOI18N

        statusMessageLabel.setName("statusMessageLabel"); // NOI18N

        statusAnimationLabel.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        statusAnimationLabel.setName("statusAnimationLabel"); // NOI18N

        progressBar.setName("progressBar"); // NOI18N

        javax.swing.GroupLayout statusPanelLayout = new javax.swing.GroupLayout(statusPanel);
        statusPanel.setLayout(statusPanelLayout);
        statusPanelLayout.setHorizontalGroup(
            statusPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(statusPanelSeparator, javax.swing.GroupLayout.DEFAULT_SIZE, 174, Short.MAX_VALUE)
            .addGroup(statusPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(statusMessageLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 91, Short.MAX_VALUE)
                .addComponent(progressBar, javax.swing.GroupLayout.PREFERRED_SIZE, 47, javax.swing.GroupLayout.PREFERRED_SIZE)
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

	/**
	 * Connects to the jabber server.
	 */
	@Action
	public void connectToServer() {
		if (conn == null) {
			// Set menu items accordingly
			connectItem.setEnabled(false);
			disconnectItem.setEnabled(false);

			// Set status bar accordingly
			statusMessageLabel.setText("Connecting...");
			progressBar.setValue(PROGRESS_BAR_MAX / 3);
			progressBar.setVisible(true);
			
			// Connect to the server (in a new thread)
			final View thisView = this;
			Thread connThread = new Thread() {
				@Override
				public void run() {
					// Create the configuration for this new connection
					ConnectionConfiguration config = new ConnectionConfiguration(
							appConfig.getHostName(), appConfig.getPort(), "jabber.org");
					config.setCompressionEnabled(false);
					config.setSelfSignedCertificateEnabled(true);
					config.setVerifyChainEnabled(false);
					config.setVerifyRootCAEnabled(false);
					config.setNotMatchingDomainCheckEnabled(false);
					SASLAuthentication.supportSASLMechanism("PLAIN", 0);
					config.setSASLAuthenticationEnabled(true);
					config.setExpiredCertificatesCheckEnabled(false);
					config.setRosterLoadedAtLogin(true);
					config.setSendPresence(true);
					if (appConfig.getUseEncryption()) {
						config.setSecurityMode(ConnectionConfiguration.SecurityMode.enabled);
					} else {
						config.setSecurityMode(ConnectionConfiguration.SecurityMode.disabled);
					}
					
					try {
						// Connect to the server
						XMPPConnection newConn = new XMPPConnection(config);
						newConn.connect();
						
						// Connected
						statusMessageLabel.setText("Logging...");
						progressBar.setValue(2 * (PROGRESS_BAR_MAX / 3));
						
						// Log to the server
						newConn.login(appConfig.getUserName(), appConfig.getUserPassword());
						
						// Connected and logged
						conn = newConn;
						
						// Create a packet filter to listen for new messages
						PacketFilter filter = new AndFilter(new PacketTypeFilter(Message.class), 
							new ToContainsFilter(appConfig.getUserName()));
						conn.addPacketListener(thisView, filter);
						
						connectItem.setEnabled(false);
						disconnectItem.setEnabled(true);
						statusMessageLabel.setText("Online!");
						progressBar.setValue(PROGRESS_BAR_MAX);
						
						generateRoster();
					} catch (XMPPException ex) {
						Dialogs.displayErrorMessage("Cannot connect or login to the server:\n" +
							appConfig.getHostName() + ":" +
							appConfig.getPort() + " (" + appConfig.getUserName() + ")\n" +
							"Error: " + ex, "Error");
						conn = null;

						// Set menu items accordingly
						connectItem.setEnabled(true);
						disconnectItem.setEnabled(false);
						
						// Set status bar accordingly
						statusMessageLabel.setText("");
						progressBar.setVisible(false);
						progressBar.setValue(PROGRESS_BAR_MIN);
					}
				}
			};
			connThread.start();
		}
	}

	/**
	 * Disconnects from the jabber server (if connected).
	 */
	@Action
	public void disconnectFromServer() {
		if (conn != null) {
			conn.removePacketListener(this);
			conn.disconnect();
			conn = null;
			
			// Set menu items accordingly
			connectItem.setEnabled(true);
			disconnectItem.setEnabled(false);
			
			// Set status bar accordingly
			statusMessageLabel.setText("");
			progressBar.setVisible(false);
			progressBar.setValue(PROGRESS_BAR_MIN);
			
			disposeAllChatWindows(true);
			generateRoster();
		}
	}

	/**
	 * Generates the roster.
	 */
	private void generateRoster() {
		// Roster initialization
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("Roster");
		roster = new JTree(root);
		roster.getSelectionModel().setSelectionMode(
				TreeSelectionModel.SINGLE_TREE_SELECTION);
		roster.addMouseListener(this);
		scrollPane.setViewportView(roster);
			
		if (conn != null) {
			// Load the roster
			Roster jabberRoster = conn.getRoster();
			Collection<RosterEntry> entries = jabberRoster.getEntries();
			if (entries.size() > 0) {
				// User icon
				ImageIcon userIcon = new ImageIcon("src/resources/roster-user-icon.png");
				DefaultTreeCellRenderer renderer = new DefaultTreeCellRenderer();
				renderer.setLeafIcon(userIcon);
				renderer.setSize(100, 100);
				roster.setCellRenderer(renderer);
		
				// Add users from the roster to the tree
				for (RosterEntry entry : entries) {
					DefaultMutableTreeNode user = new DefaultMutableTreeNode(
							entry.getUser());
					root.add(user);
				}
			}
			
			// Make all users visible
			roster.expandRow(0);
		}
	}
	
	/**
	 * Starts chat with the selected user.
	 * 
	 * @param user       User with which the chat will start.
	 */
	private void startChat(String user) {
		startChat(user, null);
	}
	
	/**
	 * Starts chat with the selected user.
	 * 
	 * @param user       User with which the chat will start.
	 * @param iniMessage Initial message (can be null).
	 */
	private synchronized void startChat(String user, String iniMessage) {
		ChatPanel chatPanel = new ChatPanel(this);
		UserChat userChat = new UserChat(chatPanel, conn, user);
		activeChatHandlerPanels.add(chatPanel);
		addChatPanelToWindow(chatPanel);
		
		// Check whether there was some initial message to be sent
		if (iniMessage != null) {
			userChat.receivedMessage(iniMessage);
		}
	}
	
	/**
	 * Adds the selected chat panel to a window. If there is no window
	 * in which the chat panel could be added, a new window will be created.
	 * 
	 * @param chatPanel Chat panel to be added.
	 */
	 private void addChatPanelToWindow(ChatPanel chatPanel) {
		 if (appConfig.getMultipleChats()) {
			// Multiple chats in one window
			// Check whether we should create a new chat window
			// or there is some existing window
			boolean chatWasAdded = false;
			if (activeMultipleChatWindows.size() > 0) {
				// There is an existing window - try to add the chat panel
				// to some window
				for (MultipleWindowChat chatWindow : activeMultipleChatWindows) {
					if (chatWindow.addChatPanel(chatPanel)) {
						chatWasAdded = true;
						chatWindow.setVisible(true);
						break;
					}
				}
			}
			
			if (!chatWasAdded) {
				// There are either no existing windows or there is no room
				// in any window - create a new window
				MultipleWindowChat chatWindow = new MultipleWindowChat(
						appConfig.getMaxMultipleChats());
				activeMultipleChatWindows.add(chatWindow);
				chatWindow.addChatPanel(chatPanel);
				chatWindow.setSize(400, 400);
				chatWindow.setMinimumSize(new Dimension(200, 200));
				chatWindow.setLocationRelativeTo(null);
				chatWindow.setVisible(true);
			}
		} else {
			// Single window chat
			SingleWindowChat chatWindow = new SingleWindowChat(chatPanel);
			activeSingleChatWindows.add(chatWindow);
			chatWindow.setSize(400, 400);
			chatWindow.setMinimumSize(new Dimension(200, 200));
			chatWindow.setLocationRelativeTo(null);
			chatWindow.setVisible(true);
		}
	}
	
	/**
	 * Disposes all active chat windows.
	 * 
	 * @param closeChats Should the chats be closed or not?
	 */
	private void disposeAllChatWindows(boolean closeChats) {
		for (SingleWindowChat chatWindow : activeSingleChatWindows) {
			chatWindow.disposeWithoutClosingChatPanel();
		}
		activeSingleChatWindows.clear();
		for (MultipleWindowChat chatWindow : activeMultipleChatWindows) {
			chatWindow.disposeWithoutClosingChatPanels();
		}
		activeMultipleChatWindows.clear();
		
		if (closeChats) {
			// The toArray() metod is needed here because of possible
			// ConcurrentModificationException exception when iterating
			// over the activeChatHandlerPanels list
			for (Object chatPanel : activeChatHandlerPanels.toArray()) {
				((ChatPanel) chatPanel).close();
			}
		} 
	} 
	 
	public void mouseClicked(MouseEvent e) {
		// Do nothing
	}

	public void mousePressed(MouseEvent e) {
		// Connection must be established
		if (conn == null) {
			return;
		}
		
		// Check for doubleclick
		int selRow = roster.getRowForLocation(e.getX(), e.getY());
		if (selRow == -1) {
			return;
		}
		
		if (e.getClickCount() == 2) {
			DefaultMutableTreeNode selNode = (DefaultMutableTreeNode)
					roster.getLastSelectedPathComponent();
			if (selNode == null) {
				// Nothing was selected
				return;
			}

			Object nodeInfo = selNode.getUserObject();
			if (selNode.isLeaf()) {
				startChat((String) nodeInfo);
			}
		}
	}

	public void mouseReleased(MouseEvent e) {
		// Do nothing
	}

	public void mouseEntered(MouseEvent e) {
		// Do nothing
	}

	public void mouseExited(MouseEvent e) {
		// Do nothing
	}
	
	public synchronized void chatPanelMessageReceived(ChatPanel chatPanelReceivedMessage) {
		if (appConfig.getMultipleChats()) {
			// Multiple chats in one window
			for (MultipleWindowChat chatWindow : activeMultipleChatWindows) {
				for (ChatPanel chatPanel : chatWindow.getChatPanels()) {
					if (chatPanel == chatPanelReceivedMessage) {
						// Blink the window
						chatWindow.toFront();
						break;
					}
				}
			}
		} else {
			// Single chat in one window
			for (SingleWindowChat chatWindow : activeSingleChatWindows) {
				if (chatWindow.getChatPanel() == chatPanelReceivedMessage) {
					// Blink the window
					chatWindow.toFront();
					break;
				}
			}
		}
	}
	
	public synchronized void chatPanelClosed(ChatPanel closedChatPanel) {
		activeChatHandlerPanels.remove(closedChatPanel);

		if (appConfig.getMultipleChats()) {
			// Multiple chats in one window
			// The toArray() metod is needed here because of possible
			// ConcurrentModificationException exception when iterating
			// over the activeMultipleChatWindows list
			for (Object chatWindow : activeMultipleChatWindows.toArray()) {
				for (ChatPanel chatPanel : ((MultipleWindowChat) chatWindow).getChatPanels()) {
					if (chatPanel == closedChatPanel) {
						((MultipleWindowChat) chatWindow).removeChatPanel(closedChatPanel);
						if (((MultipleWindowChat) chatWindow).getChatPanels().size() == 0) {
							// There are no chat panels left in this window,
							// so dispose it
							((MultipleWindowChat) chatWindow).dispose();
							activeMultipleChatWindows.remove(chatWindow);
						}
						break;
					}
				}
			}
		} else {
			// Single chat in one window
			for (SingleWindowChat chatWindow : activeSingleChatWindows) {
				if (chatWindow.getChatPanel() == closedChatPanel) {
					activeSingleChatWindows.remove(chatWindow);
					chatWindow.dispose();
					break;
				}
			}
		}
	}
	
	public synchronized void processPacket(Packet p) {
		// The registered filter is only for Message packets, so the casts are safe
		String user = ((Message) p).getFrom();
		if (user.contains("/")) {
			// Remove the resource name from the user name
			user = user.substring(0, user.indexOf("/"));
		}
		
		// If the message is part of an existing chat, don't do anything
		for (ChatPanel chatPanel : activeChatHandlerPanels) {
			UserChat userChat = chatPanel.getUserChat();
			if (userChat != null && userChat.getUser().equals(user)) {
				return;
			}
		}
		
		startChat(user, ((Message) p).getBody());
	}

	/**
	 * Toggles multiple chats in one window option.
	 */
	@Action
	public void toggleMultipleChatsInOneWindow() {
		appConfig.setMultipleChats(multipleChatsItem.getState());
				
		// Dispose all existing windows (without closing chat panels)
		disposeAllChatWindows(false);
		
		// Create new window(s) for all existing chat panels
		for (ChatPanel chatPanel : activeChatHandlerPanels) {
			addChatPanelToWindow(chatPanel);
		}
	}
	
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem configureItem;
    private javax.swing.JMenuItem connectItem;
    private javax.swing.JMenuItem disconnectItem;
    private javax.swing.JPanel mainPanel;
    private javax.swing.JMenuBar menuBar;
    private javax.swing.JCheckBoxMenuItem multipleChatsItem;
    private javax.swing.JProgressBar progressBar;
    private javax.swing.JTree roster;
    private javax.swing.JScrollPane scrollPane;
    private javax.swing.JMenu settingsMenu;
    private javax.swing.JLabel statusAnimationLabel;
    private javax.swing.JLabel statusMessageLabel;
    private javax.swing.JPanel statusPanel;
    // End of variables declaration//GEN-END:variables

    private final Timer messageTimer;
    private final Timer busyIconTimer;
    private final Icon idleIcon;
    private final Icon[] busyIcons = new Icon[15];
    private int busyIconIndex = 0;

    private JDialog aboutBox;
}
