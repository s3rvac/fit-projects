// Encoding: utf8
// Project:  ITW (2nd project)
// Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz

/*
 * Sets selected option to the selected value in users cookies.
 */
function setCookie(option, value) {
	var expires = new Date((new Date().getTime()) + 1000 * 60 * 60 * 24 * 7);
	document.cookie = escape(option) + '=' + escape(value)
		+ '; expires=' + expires.toGMTString();
}

/*
 * Gets value of the selected option in users cookies.
 */
function getCookie(option) {
	if (document.cookie != "") {
		var pos1 = document.cookie.indexOf(escape(option) + '=');
		if (pos1 != -1) {
			pos1 += escape(option).length + 1;
			var pos2 = document.cookie.indexOf(';', pos1);
			if (pos2 == -1) {
				pos2 = document.cookie.length;
			}
			return unescape(document.cookie.substring(pos1, pos2));
		}
	}

	return '';
}

/*
 * Uses users style according to the stored style in users cookies.
 */
function useUsersStyle(stylesPath) {
	var userStyleItem = document.getElementById('user_style');
	var userStyle = 'style1';
	switch (getCookie('style')) {
		case 'none':
			userStyle = 'none';
			document.getElementsByTagName('head')[0].removeChild(userStyleItem);
			return;
			break;
		case 'style2':
			userStyle = 'style2';
			break;
		default:
			break;
	}
	userStyleItem.setAttribute('href', stylesPath + userStyle + '.css');
}

/*
 * Changes users style to a new one (which is selected in formItem).
 */
function changeUsersStyle(formItem) {
	var style = 'none';
	if (formItem.styles[1].checked) {
		style = 'style1';
	} else if (formItem.styles[2].checked) {
		style = 'style2';
	}

	setCookie('style', style);
}

/*
 * Checks a button in the selected formItem according to the current users style.
 */
function checkStyleButton(formItem) {
	var style = getCookie('style');
	switch (style) {
		case 'none':
			formItem.styles[0].checked = true;
			break;
		case 'style2':
			formItem.styles[2].checked = true;
			break;
		default:
			formItem.styles[1].checked = true;
			break;
	}
}

/*
 * Changes users greeting to the selected one.
 */
function changeGreeting(greeting) {
	setCookie('greeting', greeting);
}

/*
 * Initializes various things on page load.
 */
function init() {
	// Display greeting
	var greeting = getCookie('greeting');
	if (!greeting) {
		greeting = 'návštěvníku';
	}
	var greetingItem = document.getElementById('greeting');
	greetingItem.innerHTML = 'Vítej, ' + greeting + '!';

	// Change style form
	var changeStyleForm = document.getElementById('change_style');
	if (changeStyleForm) {
		checkStyleButton(changeStyleForm);
	}

	// Change greeting form
	var greetingForm = document.getElementById('change_greeting');
	if (greetingForm) {
		greetingForm.greeting.value = greeting;
	}
}
