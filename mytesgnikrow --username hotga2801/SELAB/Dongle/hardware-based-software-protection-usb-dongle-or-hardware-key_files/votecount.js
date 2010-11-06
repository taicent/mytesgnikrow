				var key = "_http:\/\/www.pressreleasepoint.com\/hardware-based-software-protection-usb-dongle-or-hardware-key_us_en-US_square";
				var voteHtml = "vote<br>now";

				if(typeof(window.yahooBuzzBadgeVoteCountValues) !== "undefined") {
					window.yahooBuzzBadgeVoteCountValues[key] = voteHtml;	
				} else {
					window.yahooBuzzBadgeVoteCountValues = {};
				}

				if(typeof(window.yahooBuzzBadgeVoteCountIds) !== "undefined") {
					var ids = window.yahooBuzzBadgeVoteCountIds[key];
					if(typeof(ids) !== "undefined") {
						var numIds = ids.length;
						for(var i = 0; i < numIds; i++) {
							if(document.getElementById(ids[i])) {
								document.getElementById(ids[i]).innerHTML = voteHtml;
							}
						}
					}
				}