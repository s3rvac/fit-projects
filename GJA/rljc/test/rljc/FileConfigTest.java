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

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Testing unit for the FileConfig class.
 */
public class FileConfigTest {

	/** Configuration file. */
	File configFile = null;
	
    public FileConfigTest() {
    }

	@BeforeClass
	public static void setUpClass() throws Exception {
	}

	@AfterClass
	public static void tearDownClass() throws Exception {
	}

    @Before
    public void setUp() throws IOException {
		configFile = File.createTempFile("FileConfigTest", ".test");
    }

    @After
    public void tearDown() {
		configFile.delete();
    }

	/**
	 * Tries to create a configuration file from a nonexisting file.
	 */
	@Test
	public void constructFromNonExistingFile() {
		try {
			FileConfig fc = FileConfig.fromFile(new File("FileConfigTestFile"));
			assertTrue(false); // Should have thrown an exception
		} catch (IOException ex) {
			// Ok, the test passed
		}
	}
	
	/**
	 * Tries to create a configuration file from an existing empty file.
	 */
	@Test
	public void constructFromExistingEmptyFile() throws IOException {
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(FileConfig.DEFAULT_HOST_NAME, fc.getHostName());
		assertEquals(FileConfig.DEFAULT_PORT, fc.getPort());
		assertEquals(FileConfig.DEFAULT_USER_NAME, fc.getUserName());
		assertEquals(FileConfig.DEFAULT_USER_PASSWORD, fc.getUserPassword());
		assertEquals(FileConfig.DEFAULT_USE_ENCRYPTION, fc.getUseEncryption());
		assertEquals(FileConfig.DEFAULT_MULTIPLE_CHATS, fc.getMultipleChats());
		assertEquals(FileConfig.DEFAULT_MAX_MULTIPLE_CHATS, fc.getMaxMultipleChats());
		assertEquals(FileConfig.DEFAULT_APP_WIDTH, fc.getAppWidth());
		assertEquals(FileConfig.DEFAULT_APP_HEIGHT, fc.getAppHeight());
		assertEquals(FileConfig.DEFAULT_APP_X_POS, fc.getAppXPos());
		assertEquals(FileConfig.DEFAULT_APP_Y_POS, fc.getAppYPos());
	}

	/**
	 * Tests host name parsing.
	 */
	@Test
	public void hostNameParsing() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "host-name = jabber.cz";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals("jabber.cz", fc.getHostName());
	}
	
	/**
	 * Tests port parsing - additional white spaces in the line.
	 */
	@Test
	public void portParsingAdditionalSpaces() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "\tport  = 2\t\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(2, fc.getPort());
	}
	
	/**
	 * Tests port parsing - too low port number.
	 */
	@Test
	public void portParsingTooLow() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "port=-257\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests port parsing - zero.
	 */
	@Test
	public void portParsingZero() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "port=0\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests port parsing - valid prot number.
	 */
	@Test
	public void portParsingValid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "port=10456\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(10456, fc.getPort());
	}
	
	/**
	 * Tests port parsing - too high port number.
	 */
	@Test
	public void portParsingTooHigh() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "port=65536\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests user name parsing.
	 */
	@Test
	public void userNameParsing() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "user-name = longh";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals("longh", fc.getUserName());
	}
	
	/**
	 * Tests user password parsing.
	 */
	@Test
	public void userPasswordParsing() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "user-password = x7$dkDs2";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals("x7$dkDs2", fc.getUserPassword());
	}
	
	/**
	 * Tests use encryption parsing - true.
	 */
	@Test
	public void useEncryptionParsingTrue() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "use-encryption = true";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(true, fc.getUseEncryption());
	}
	
	/**
	 * Tests use encryption parsing - false.
	 */
	@Test
	public void useEncryptionParsingFalse() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "use-encryption = false";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(false, fc.getUseEncryption());
	}
	
	/**
	 * Tests use encryption parsing - invalid value.
	 */
	@Test
	public void useEncryptionParsingInvalid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "use-encryption = xyz";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests multiple chats in one window parsing - true.
	 */
	@Test
	public void multipleChatsInOneWindowParsingTrue() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "multiple-chats = true";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(true, fc.getMultipleChats());
	}
	
	/**
	 * Tests multiple chats in one window parsing - false.
	 */
	@Test
	public void multipleChatsInOneWindowParsingFalse() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "multiple-chats = false";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(false, fc.getMultipleChats());
	}
	
	/**
	 * Tests multiple chats in one window parsing - invalid value.
	 */
	@Test
	public void multipleChatsInOneWindowInvalid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "multiple-chats = #$%#%";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests maximal multiple chats parsing - additional white spaces in the line.
	 */
	@Test
	public void maxMultipleChatsParsingAdditionalSpaces() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "\tmax-multiple-chats  = 2\t\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(2, fc.getMaxMultipleChats());
	}
	
	/**
	 * Tests maximal multiple chats parsing - too low port number.
	 */
	@Test
	public void maxMultipleChatsParsingTooLow() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "max-multiple-chats=0\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests maximal multiple chats parsing - valid prot number.
	 */
	@Test
	public void maxMultipleChatsParsingValid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "max-multiple-chats=10\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(10, fc.getMaxMultipleChats());
	}
	
	/**
	 * Tests port parsing - too high port number.
	 */
	@Test
	public void maxMultipleChatsParsingTooHigh() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "max-multiple-chats=17\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests application window width - too low width.
	 */
	@Test
	public void appWidthParsingTooLow() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-width=-100\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests application window width - valid width.
	 */
	@Test
	public void appWidthParsingValid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-width=600\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(600, fc.getAppWidth());
	}
	
	/**
	 * Tests application window height - too low height.
	 */
	@Test
	public void appHeightParsingTooLow() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-height=-4\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests application window height - valid height.
	 */
	@Test
	public void appHeightParsingValid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-height=200\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(200, fc.getAppHeight());
	}
	
	/**
	 * Tests application window x position - too low position.
	 */
	@Test
	public void appXPosParsingTooLow() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-x-pos=-9999\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests application window x position - valid position.
	 */
	@Test
	public void appXPosParsingValid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-x-pos=167\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(167, fc.getAppXPos());
	}
	
	/**
	 * Tests application window y position - too low position.
	 */
	@Test
	public void appYPosParsingTooLow() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-y-pos=-1\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		assertEquals(null, FileConfig.fromFile(configFile));
	}
	
	/**
	 * Tests application window y position - valid position.
	 */
	@Test
	public void appYPosParsingValid() throws IOException {
		FileWriter fw = new FileWriter(configFile);
		String fcData = "app-y-pos=222\n";
		fw.write(fcData, 0, fcData.length());
		fw.close();
		FileConfig fc = FileConfig.fromFile(configFile);
		assertEquals(222, fc.getAppYPos());
	}
	
	/**
	 * Tests configuration storage.
	 */
	@Test
	public void saveConfiguration() throws IOException {
		FileConfig fc = FileConfig.fromFile(configFile);
		fc.setHostName("test-hn");
		fc.setPort(244);
		fc.setUserName("test-un");
		fc.setUserPassword("test-up");
		fc.setUseEncryption(false);
		fc.setMultipleChats(true);
		fc.setMaxMultipleChats(3);
		fc.setAppWidth(100);
		fc.setAppHeight(200);
		fc.setAppXPos(22);
		fc.setAppYPos(23);
		assertTrue(fc.saveConfiguration());
		
		FileConfig nfc = FileConfig.fromFile(configFile);
		assertEquals("test-hn", nfc.getHostName());
		assertEquals(244, nfc.getPort());
		assertEquals("test-un", nfc.getUserName());
		assertEquals("test-up", nfc.getUserPassword());
		assertEquals(false, nfc.getUseEncryption());
		assertEquals(true, nfc.getMultipleChats());
		assertEquals(3, nfc.getMaxMultipleChats());
		assertEquals(100, nfc.getAppWidth());
		assertEquals(200, nfc.getAppHeight());
		assertEquals(22, nfc.getAppXPos());
		assertEquals(23, nfc.getAppYPos());
	}
}
