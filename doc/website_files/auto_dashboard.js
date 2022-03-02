
        var Logger = {
            log: function() {
                var line = Array.prototype.slice.call(arguments).map(function(argument) {
                    return typeof argument === 'string' ? argument : JSON.stringify(argument);
                }).join(' ');

                document.querySelector('#log').textContent = '> ' + line + '\n' + document.querySelector('#log').textContent;
            },

            clearLog: function() {
                document.querySelector('#log').textContent = '';
            },
        };

	// takes any valid JSON string and sends all key-value pairs to the screen 
	// (inside the div with id:  "masterDivId"
	function parseAndDisplay( jsonString ) { 
	  	console.log("parseAndDisplay");
                var obj = JSON.parse(jsonString);
		for (var key in obj) {
	  		console.log(`key-value:  ${key}:${obj[key]}`);
			let masterDiv =  document.getElementById("masterDivId");
			let targetValDiv =  document.getElementById(key+"valDivId");
			if ( !targetValDiv ) {
				console.log("no target Div");
				let newKeyValDiv = document.createElement('div');
				let newKeyDiv = document.createElement('div');
				let newValDiv = document.createElement('div');

				newKeyValDiv.setAttribute('id',key + "keyValDivId");
				newKeyDiv.setAttribute('id',key + "keyDivId");
				newValDiv.setAttribute('id',key + "valDivId");

				masterDiv.appendChild(newKeyValDiv);
				newKeyValDiv.appendChild(newKeyDiv);
				newKeyValDiv.appendChild(newValDiv);

				newKeyDiv.classList.add("headerBars");
				newValDiv.classList.add("boxValue");

				newKeyDiv.innerHTML = key;
				newValDiv.innerHTML = obj[key];

			} else {
				targetValDiv.innerHTML = obj[key];
			}
		}

	}

 	function randBetween( min, max ) {
		  return Math.floor(Math.random() * (max - min)) + min + 1;
	}


	
        // for testing only
	 setInterval(function(){ parseAndDisplay('{"key1":"' + randBetween(0,100) + '", "key2":"' + randBetween(0,100) + '"}'); }, 1000);
	 setInterval(function(){ parseAndDisplay('{"key1":"' + randBetween(0,100) + '", "key3":"' + randBetween(0,100) + '"}'); }, 2000);

