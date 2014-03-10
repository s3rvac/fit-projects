/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Window where users can search photos based on photo similarity
 *        or description.
 *
 */

package zoo;

import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;

/**
 * @brief Window where users can search photos based on photo similarity
 *        or description.
 */
public class PhotoSearchWindow extends javax.swing.JFrame {

	/**
	 * Offset that should be added to the uploaded photo size when displaying it.
	 * It is here because without it, when a photo is displayed, the photo
	 * box is moved to the left. Don't ask me why, I don't know it.
	 */
	private static final int UPLOADED_PHOTO_SIZE_OFFSET = -4;

	/// Result table columns.
	private static final String[] RESULT_TABLE_COLUMNS = {
		"Species",
		"Description",
		"Photo"
	};

	/// Result table columns for a table with no results.
	private static final String[] RESULT_TABLE_NO_RESULTS_COLUMNS = {
		"No results"
	};

	/// Table row index of the animal location.
	private static final int SPECIES_TABLE_ROW_INDEX = 0;

	/// Table row index of the animal description.
	private static final int DESCRIPTION_TABLE_ROW_INDEX = 1;

	/// Table row index of the animal photo.
	private static final int PHOTO_TABLE_ROW_INDEX = 2;

	/// Result set row of the animal species.
	private static final int SPECIES_RS_ROW_INDEX = 2;

	/// Result set row of the animal description.
	private static final int DESCRIPTION_RS_ROW_INDEX = 3;

	/// Result set row of the animal photo.
	private static final int PHOTO_RS_ROW_INDEX = 4;

	/// Size of a photo in the result table.
	private static final int RESULT_TABLE_PHOTO_SIZE = 32;

	/// Raw uploaded photo (i.e. without any transformations).
	private BufferedImage _rawUploadedPhoto = null;

	/// ID of the uploaded photo.
	private int _uploadedPhotoId = PhotoOperations.NO_UPLOADED_PHOTO_ID;

	/**
	 * @brief Shows a file dialog to set a new photo and then uploads it.
	 *
	 * If the selected photo is uploaded successfully, it will be displayed.
	 * Otherwise, error message will be shown.
	 */
	private void getPhotoAndUploadIt() {
		File selectedFile = Dialogs.getFileFromUser(this);
		if (selectedFile != null) {
			try {
				// Try to load the photo
				_rawUploadedPhoto = ImageIO.read(selectedFile);
				if (_rawUploadedPhoto == null) {
					Dialogs.displayErrorMessage(this, "Selected photo type is not supported.",
							"Invalid photo type");
					return;
				}

				// Upload it to the db
				_uploadedPhotoId = PhotoOperations.uploadTemporaryPhotoToDb(
						selectedFile.getAbsolutePath());

				// Convert the photo and display it
				selectedPhoto.setIcon(new ImageIcon(_rawUploadedPhoto.getScaledInstance(
						selectedPhoto.getWidth() + UPLOADED_PHOTO_SIZE_OFFSET,
						selectedPhoto.getHeight() + UPLOADED_PHOTO_SIZE_OFFSET, 0)));
			} catch (IOException ex) {
				Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
						ex.getClass() + "): " + ex.getMessage());
				Dialogs.displayErrorMessage(this, "Selected photo could not be uploaded." +
						"\nReason: " + ex.getMessage(), "Error during photo upload");
				_rawUploadedPhoto = null;
			} catch (SQLException ex) {
				Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
						ex.getClass() + "): " + ex.getMessage());
				Dialogs.displayErrorMessage(this, "Selected photo could not be uploaded." +
						"\nReason: " + ex.getMessage(), "Error during photo upload");
				_rawUploadedPhoto = null;
			}
		}
	}

	/**
	 * @brief Searches for photos by the selected mode.
	 *
	 * Mode depends on the currently selected tab.
	 */
	private void searchPhotos() {
		if (tabbedPane.getSelectedComponent() == simSearchPanel) {
			if (_rawUploadedPhoto == null) {
				Dialogs.displayErrorMessage(this,
						"You have to first select some photo.", "No photo selected");
				return;
			}

			searchPhotosBySimilarity();
		} else if (tabbedPane.getSelectedComponent() == descSearchPanel) {
			searchPhotosByDescription(photoDesc.getText());
		}
	}

	/**
	 * @brief Searches the database for photos that are similar to the uploaded
	 *        photo.
	 */
	private void searchPhotosBySimilarity() {
		try {
			String query = "SELECT a.animal_id, a.species, a.description, a.photo," +
					"ORDSYS.IMGScore(123) AS distance " +
				"FROM animal a, uploaded_photo up " +
				"WHERE ORDSYS.IMGSimilar(a.photo_signature, up.photo_signature, " +
					"'color=\"0.3\" texture=\"0.3\" shape=\"0.3\" location=\"0.1\"', 100, 123) < 100 " +
					"AND up.uploaded_photo_id = " + Integer.toString(_uploadedPhotoId) + " " +
				"ORDER BY distance";
			OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(query);
			fillResultsTable(rs);
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}
	}

	/**
	 * @brief Searches the database for photos that match the selected
	 *        description.
	 *
	 * @param[in] description Description of the searched photo.
	 */
	private void searchPhotosByDescription(String description) {
		try {
			// TODO - use some more sophisticated query
			// TODO - escape the description string (defense againts SQL injection attacks)
			//      - see http://www.lc.leidenuniv.nl/awcourse/oracle/text.920/a96518/cqspcl.htm

			// Case insensitive searching (using UPPER() and toUpperCase())
			String query = "SELECT animal_id, species, description, photo " +
				"FROM animal " +
				"WHERE UPPER(description) LIKE '%" + description.toUpperCase() + "%'";
			OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(query);
			fillResultsTable(rs);
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}
	}

	/**
	 * @brief Fills the result table with the selected result set (including photos).
	 *
	 * @param rs Source data result set.
	 *
	 * Passed result set must have the following columns exactly in this order:
	 *  - Animal ID (NUMBER)
	 *  - Species (VARCHAR)
	 *  - Description (VARCHAR)
	 *  - Photo (ORDSYS.ORDIMAGE)
	 *
	 * There can be additional columns, but this method will not process them.
	 */
	private void fillResultsTable(OracleResultSet rs) {
		try {
			// Check the result set
			if (rs == null || rs.isClosed()) {
				throw new SQLException("Passed result set is invalid.");
			}

			// Check if there are some results
			if (rs.next()) {
				// Set table header and create a data model
				ResultTableModel tableModel = new ResultTableModel(
						RESULT_TABLE_COLUMNS, false);

				// There are some results, process them
				do {
					Object[] row = new Object[RESULT_TABLE_COLUMNS.length];

					// Species
					row[SPECIES_TABLE_ROW_INDEX] = rs.getString(SPECIES_RS_ROW_INDEX);

					// Description
					row[DESCRIPTION_TABLE_ROW_INDEX] = rs.getString(DESCRIPTION_RS_ROW_INDEX);

					// Photo
					BufferedImage photo = Conversions.ordImageToBufferedImage(
							(OrdImage) rs.getORAData(PHOTO_RS_ROW_INDEX,
									OrdImage.getORADataFactory()));

					// Convert to small RGB image properly
					resultsTable.setRowHeight(RESULT_TABLE_PHOTO_SIZE);
					BufferedImage photoThumb = new BufferedImage(
							RESULT_TABLE_PHOTO_SIZE,
							RESULT_TABLE_PHOTO_SIZE,
							BufferedImage.TYPE_INT_ARGB);
					Graphics graphics = photoThumb.getGraphics();
					graphics.drawImage(photo,
						0, 0, photoThumb.getWidth(), photoThumb.getHeight(),
						0, 0, photo.getWidth(), photo.getWidth(),
						null);

					// Create an icon from the photo and add it to the table
					row[PHOTO_TABLE_ROW_INDEX] = new ImageIcon(photoThumb);

					// Add the row to the table
					tableModel.addRow(row);
				} while (rs.next());

				resultsTable.setModel(tableModel);
			} else {
				// There are no results
				ResultTableModel tableModel = new ResultTableModel(
						RESULT_TABLE_NO_RESULTS_COLUMNS);
				resultsTable.setModel(tableModel);
			}

			resultsTable.invalidate();
		} catch (OutOfMemoryError ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}
	}

	/**
	 * @brief Creates new form PhotoSearchWindow.
	 */
	public PhotoSearchWindow() {
		initComponents();
	}

	/** This method is called from within the constructor to
	 * initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is
	 * always regenerated by the Form Editor.
	 */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        tabbedPane = new javax.swing.JTabbedPane();
        simSearchPanel = new javax.swing.JPanel();
        simPanelDescPane = new javax.swing.JScrollPane();
        simPaneDescText = new javax.swing.JTextArea();
        selectedPhoto = new javax.swing.JLabel();
        selectPhotoButton = new javax.swing.JButton();
        descSearchPanel = new javax.swing.JPanel();
        photoDescPane = new javax.swing.JScrollPane();
        photoDesc = new javax.swing.JTextArea();
        panelDescPane = new javax.swing.JScrollPane();
        descPaneDescText = new javax.swing.JTextArea();
        resultsPane = new javax.swing.JScrollPane();
        resultsTable = new javax.swing.JTable();
        searchButton = new javax.swing.JButton();

        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(zoo.ZooApp.class).getContext().getResourceMap(PhotoSearchWindow.class);
        setTitle(resourceMap.getString("Form.title")); // NOI18N
        setName("Form"); // NOI18N

        tabbedPane.setName("tabbedPane"); // NOI18N

        simSearchPanel.setName("simSearchPanel"); // NOI18N

        simPanelDescPane.setHorizontalScrollBarPolicy(javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        simPanelDescPane.setVerticalScrollBarPolicy(javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
        simPanelDescPane.setName("simPanelDescPane"); // NOI18N

        simPaneDescText.setBackground(resourceMap.getColor("simPaneDescText.background")); // NOI18N
        simPaneDescText.setColumns(20);
        simPaneDescText.setEditable(false);
        simPaneDescText.setLineWrap(true);
        simPaneDescText.setRows(5);
        simPaneDescText.setText(resourceMap.getString("simPaneDescText.text")); // NOI18N
        simPaneDescText.setWrapStyleWord(true);
        simPaneDescText.setAutoscrolls(false);
        simPaneDescText.setName("simPaneDescText"); // NOI18N
        simPanelDescPane.setViewportView(simPaneDescText);

        selectedPhoto.setText(resourceMap.getString("selectedPhoto.text")); // NOI18N
        selectedPhoto.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        selectedPhoto.setDoubleBuffered(true);
        selectedPhoto.setName("selectedPhoto"); // NOI18N
        selectedPhoto.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                selectedPhotoMouseClicked(evt);
            }
        });

        selectPhotoButton.setText(resourceMap.getString("selectPhotoButton.text")); // NOI18N
        selectPhotoButton.setName("selectPhotoButton"); // NOI18N
        selectPhotoButton.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                selectPhotoButtonMouseClicked(evt);
            }
        });
        selectPhotoButton.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                selectPhotoButtonKeyPressed(evt);
            }
        });

        javax.swing.GroupLayout simSearchPanelLayout = new javax.swing.GroupLayout(simSearchPanel);
        simSearchPanel.setLayout(simSearchPanelLayout);
        simSearchPanelLayout.setHorizontalGroup(
            simSearchPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(simPanelDescPane, javax.swing.GroupLayout.DEFAULT_SIZE, 263, Short.MAX_VALUE)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, simSearchPanelLayout.createSequentialGroup()
                .addContainerGap(87, Short.MAX_VALUE)
                .addComponent(selectPhotoButton)
                .addGap(82, 82, 82))
            .addGroup(simSearchPanelLayout.createSequentialGroup()
                .addGap(66, 66, 66)
                .addComponent(selectedPhoto, javax.swing.GroupLayout.PREFERRED_SIZE, 128, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(69, Short.MAX_VALUE))
        );
        simSearchPanelLayout.setVerticalGroup(
            simSearchPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(simSearchPanelLayout.createSequentialGroup()
                .addComponent(simPanelDescPane, javax.swing.GroupLayout.PREFERRED_SIZE, 62, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(selectedPhoto, javax.swing.GroupLayout.PREFERRED_SIZE, 96, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(12, 12, 12)
                .addComponent(selectPhotoButton)
                .addContainerGap(48, Short.MAX_VALUE))
        );

        selectedPhoto.getAccessibleContext().setAccessibleName(resourceMap.getString("uploadedImage.AccessibleContext.accessibleName")); // NOI18N

        tabbedPane.addTab(resourceMap.getString("simSearchPanel.TabConstraints.tabTitle"), simSearchPanel); // NOI18N

        descSearchPanel.setName("descSearchPanel"); // NOI18N

        photoDescPane.setName("photoDescPane"); // NOI18N

        photoDesc.setColumns(20);
        photoDesc.setRows(5);
        photoDesc.setName("photoDesc"); // NOI18N
        photoDescPane.setViewportView(photoDesc);

        panelDescPane.setHorizontalScrollBarPolicy(javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        panelDescPane.setVerticalScrollBarPolicy(javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
        panelDescPane.setName("panelDescPane"); // NOI18N

        descPaneDescText.setBackground(resourceMap.getColor("descPaneDescText.background")); // NOI18N
        descPaneDescText.setColumns(20);
        descPaneDescText.setEditable(false);
        descPaneDescText.setLineWrap(true);
        descPaneDescText.setRows(5);
        descPaneDescText.setText(resourceMap.getString("descPaneDescText.text")); // NOI18N
        descPaneDescText.setWrapStyleWord(true);
        descPaneDescText.setAutoscrolls(false);
        descPaneDescText.setName("descPaneDescText"); // NOI18N
        panelDescPane.setViewportView(descPaneDescText);

        javax.swing.GroupLayout descSearchPanelLayout = new javax.swing.GroupLayout(descSearchPanel);
        descSearchPanel.setLayout(descSearchPanelLayout);
        descSearchPanelLayout.setHorizontalGroup(
            descSearchPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(panelDescPane, javax.swing.GroupLayout.DEFAULT_SIZE, 263, Short.MAX_VALUE)
            .addComponent(photoDescPane, javax.swing.GroupLayout.DEFAULT_SIZE, 263, Short.MAX_VALUE)
        );
        descSearchPanelLayout.setVerticalGroup(
            descSearchPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, descSearchPanelLayout.createSequentialGroup()
                .addComponent(panelDescPane, javax.swing.GroupLayout.DEFAULT_SIZE, 63, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(photoDescPane, javax.swing.GroupLayout.PREFERRED_SIZE, 184, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        tabbedPane.addTab(resourceMap.getString("descSearchPanel.TabConstraints.tabTitle"), descSearchPanel); // NOI18N

        resultsPane.setName("resultsPane"); // NOI18N

        resultsTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null}
            },
            new String [] {
                "Species", "Description", "Photo"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.Object.class, java.lang.String.class, java.lang.Object.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        resultsTable.setColumnSelectionAllowed(true);
        resultsTable.setName("resultsTable"); // NOI18N
        resultsPane.setViewportView(resultsTable);
        resultsTable.getColumnModel().getSelectionModel().setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);

        searchButton.setText(resourceMap.getString("searchButton.text")); // NOI18N
        searchButton.setName("searchButton"); // NOI18N
        searchButton.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                searchButtonMouseClicked(evt);
            }
        });
        searchButton.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                searchButtonKeyPressed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(tabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, 268, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(100, 100, 100)
                        .addComponent(searchButton)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(resultsPane, javax.swing.GroupLayout.DEFAULT_SIZE, 427, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addComponent(tabbedPane, javax.swing.GroupLayout.DEFAULT_SIZE, 278, Short.MAX_VALUE)
                .addGap(18, 18, 18)
                .addComponent(searchButton)
                .addGap(26, 26, 26))
            .addGroup(layout.createSequentialGroup()
                .addGap(12, 12, 12)
                .addComponent(resultsPane, javax.swing.GroupLayout.DEFAULT_SIZE, 333, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

	private void selectedPhotoMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_selectedPhotoMouseClicked
		getPhotoAndUploadIt();
}//GEN-LAST:event_selectedPhotoMouseClicked

	private void selectPhotoButtonMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_selectPhotoButtonMouseClicked
		// Was it a left mouse button click?
		if ((evt.getModifiers() & MouseEvent.BUTTON1_MASK) == MouseEvent.BUTTON1_MASK) {
			getPhotoAndUploadIt();
		}
}//GEN-LAST:event_selectPhotoButtonMouseClicked

	private void selectPhotoButtonKeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_selectPhotoButtonKeyPressed
		getPhotoAndUploadIt();
}//GEN-LAST:event_selectPhotoButtonKeyPressed

	private void searchButtonKeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_searchButtonKeyPressed
		searchPhotos();
	}//GEN-LAST:event_searchButtonKeyPressed

	private void searchButtonMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_searchButtonMouseClicked
		// Was it a left mouse button click?
		if ((evt.getModifiers() & MouseEvent.BUTTON1_MASK) == MouseEvent.BUTTON1_MASK) {
			searchPhotos();
		}
	}//GEN-LAST:event_searchButtonMouseClicked

	/**
	 * @param args the command line arguments
	 */
	public static void main(String args[]) {
		java.awt.EventQueue.invokeLater(new Runnable() {
			public void run() {
				new PhotoSearchWindow().setVisible(true);
			}
		});
	}

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTextArea descPaneDescText;
    private javax.swing.JPanel descSearchPanel;
    private javax.swing.JScrollPane panelDescPane;
    private javax.swing.JTextArea photoDesc;
    private javax.swing.JScrollPane photoDescPane;
    private javax.swing.JScrollPane resultsPane;
    private javax.swing.JTable resultsTable;
    private javax.swing.JButton searchButton;
    private javax.swing.JButton selectPhotoButton;
    private javax.swing.JLabel selectedPhoto;
    private javax.swing.JTextArea simPaneDescText;
    private javax.swing.JScrollPane simPanelDescPane;
    private javax.swing.JPanel simSearchPanel;
    private javax.swing.JTabbedPane tabbedPane;
    // End of variables declaration//GEN-END:variables

}
