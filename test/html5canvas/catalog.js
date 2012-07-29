var catalog = {
    create: function(collections) {
		var parent = document.getElementsByTagName('body')[0];
		for (var i in collections) {
			var h1 = document.createElement('h1');
			parent.appendChild(h1);
			h1.innerHTML = i;

			var ul = document.createElement('ul');
			parent.appendChild(ul);

			var collection = collections[i];

			for (var j in collection.elements) {
				var element = collection.elements[j];
				var li = document.createElement('li');
				ul.appendChild(li);

				var idDiv = document.createElement('div');
				idDiv.innerHTML = element.id;

				var canvasWidth = 200;
				var canvasHeight = 200;
				var widthToHeightRatio = element.bounds.width / element.bounds.height;
				var scaleFactor = Math.min(canvasWidth / element.bounds.width, canvasHeight / element.bounds.height);
				var renderWidth = element.bounds.width * scaleFactor;
				var renderHeight = element.bounds.height * scaleFactor;

				var canvas = document.createElement('canvas');
				li.appendChild(canvas);
				var context = canvas.getContext('2d');
				canvas.width = canvasWidth;
				canvas.height = canvasHeight;

				collection.draw(context, j, (canvasWidth - renderWidth) / 2, (canvasHeight - renderHeight) / 2, renderWidth, renderHeight)

				var idDiv = document.createElement('div');
				idDiv.innerHTML = element.id;
				li.appendChild(idDiv);
			}
		}
	}
}
