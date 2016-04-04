define(["knockout", "idd"], function (ko, idd) {
    ko.bindingHandlers.iddY = {
        update: function (element, valueAccessor, allBindings, viewModel, bindingContext) {
            var value = valueAccessor();
            var unwrappedY = ko.unwrap(value);

            var xBindings;
            if (!allBindings.has('iddX'))
                throw new Error("Please define iddX binding along with iddY");
            else
                xBindings = allBindings.get('iddX');
            var stroke = undefined;
            if (allBindings.has('iddStroke')) {
                var strokeBinding = allBindings.get('iddStroke');
                stroke = ko.unwrap(strokeBinding);
            }
            var unwrappedX = ko.unwrap(xBindings);
            var plotAttr = element.getAttribute("data-idd-plot");
            if (plotAttr != null) {
                if (typeof element.plot != 'undefined') {
                    var data = { x: unwrappedX, y: unwrappedY };
                    if (typeof stroke != 'undefined')
                        data.stroke = stroke;
                    element.plot.draw(data);
                }
                else { //the case when the element was not yet initialized and not yet bound to the logical entity (plot)
                    //storing the data in the DOM. it will be used by IDD during IDD-initializing of the dom element
                    var csvDataToDraw = "x y";
                    var len = unwrappedX.length;
                    for (var i = 0; i < len; i++) {
                        csvDataToDraw += "\n"+unwrappedX[i] + " " + unwrappedY[i];
                    }
                    element.innerHTML = csvDataToDraw;
                    
                    if (typeof stroke != 'undefined') {
                        //saving stroke color in the data-idd-style attribute: will be picked up by initialization
                        element.setAttribute("data-idd-style", "stroke: " + stroke);
                    }
                }
            }
        }
    };

    ko.bindingHandlers.iddAreaY1 = {
        update: function (element, valueAccessor, allBindings, viewModel, bindingContext) {
            var value = valueAccessor();
            var unwrappedY1 = ko.unwrap(value);

            var xBindings;
            if (!allBindings.has('iddX'))
                throw new Error("Please define iddX binding along with iddAreaY1");
            else
                xBindings = allBindings.get('iddX');
            var unwrappedX = ko.unwrap(xBindings);
            var y2Bindings;
            if (!allBindings.has('iddAreaY2'))
                throw new Error("Please define iddAreaY2 binding along with iddAreaY1");
            else
                y2Bindings = allBindings.get('iddAreaY2');
            var unwrappedY2 = ko.unwrap(y2Bindings);
            var fillBindings;
            var unwrappedFill;
            if (!allBindings.has('iddAreaFill'))
                unwrappedFill = undefined;
            else
                fillBindings = allBindings.get('iddAreaFill');
            var unwrappedFill = ko.unwrap(fillBindings);
            var plotAttr = element.getAttribute("data-idd-plot");
            if (plotAttr != null) {
                if (typeof element.plot !== 'undefined') {
                    var data = { x: unwrappedX, y1: unwrappedY1, y2: unwrappedY2 };
                    if (typeof unwrappedFill !== 'undefined')
                        data.fill = unwrappedFill;
                    element.plot.draw(data);
                }
                else { //the case when the element was not yet initialized and not yet bound to the logical entity (plot)
                    //storing the data in the DOM. it will be used by IDD during IDD-initializing of the dom element
                    var csvDataToDraw = "x y1 y2";
                    var len = unwrappedX.length;
                    for (var i = 0; i < len; i++) {
                        csvDataToDraw += "\n" + unwrappedX[i] + " " + unwrappedY1[i] + " " + unwrappedY2[i];
                    }
                    element.innerHTML = csvDataToDraw;

                    if (typeof unwrappedFill !== 'undefined') {
                        //saving stroke color in the data-idd-style attribute: will be picked up by initialization
                        element.setAttribute("data-idd-style", "fill: " + unwrappedFill);
                    }

                }
            }
        }
    };

    var barMarker = {
        draw: function (marker, plotRect, screenSize, transform, context) {
            var barWidth = 0.5 * marker.barWidth;
            var xLeft = transform.dataToScreenX(marker.x - barWidth);
            var xRight = transform.dataToScreenX(marker.x + barWidth);
            if (xLeft > screenSize.width || xRight < 0) return;
            var yTop = transform.dataToScreenY(marker.y);
            var yBottom = transform.dataToScreenY(0);
            if (yTop > yBottom) {
                var k = yBottom;
                yBottom = yTop;
                yTop = k;
            }
            if (yTop > screenSize.height || yBottom < 0) return;

            if (marker.shadow) {
                context.fillStyle = marker.shadow;
                context.fillRect(xLeft + 2, yTop + 2, xRight - xLeft, yBottom - yTop);
            }

            context.fillStyle = marker.color;
            context.fillRect(xLeft, yTop, xRight - xLeft, yBottom - yTop);
            if (marker.border) {
                context.strokeStyle = marker.border;
                context.strokeRect(xLeft, yTop, xRight - xLeft, yBottom - yTop);
            }
        },

        getBoundingBox: function (marker) {
            var barWidth = marker.barWidth;
            var xLeft = marker.x - barWidth / 2;
            var yBottom = Math.min(0, marker.y);
            return { x: xLeft, y: yBottom, width: barWidth, height: Math.abs(marker.y) };
        },

        hitTest: function (marker, transform, ps, pd) {
            var barWidth = marker.barWidth;
            var xLeft = marker.x - barWidth / 2;
            var yBottom = Math.min(0, marker.y);
            if (pd.x < xLeft || pd.x > xLeft + barWidth) return false;
            if (pd.y < yBottom || pd.y > yBottom + Math.abs(marker.y)) return false;
            return true;
        }
    };
    var colorPalette = InteractiveDataDisplay.ColorPalette.parse("#69D2E7,#A7DBD8,#E0E4CC,#F38630,#FA6900");

    ko.bindingHandlers.iddBarChartY = {        
        update: function (element, valueAccessor, allBindings, viewModel, bindingContext) {
            

            var value = valueAccessor();
            var unwrappedY = ko.unwrap(value);

            

            var plotAttr = element.getAttribute("data-idd-plot");
            if (plotAttr != null) {
                if (typeof element.plot !== 'undefined') {
                    //taking control of visible rect.
                    var maxBar = Number.NEGATIVE_INFINITY;
                    if (typeof element.plot.maxBar !== 'undefined')
                        maxBar = element.plot.maxBar;
                    var len = unwrappedY.length;
                    var x = new Array(len);
                    var heightIncreased= false;
                    for (var i = 0; i < len; i++)
                    {
                        x[i] = i;
                        if(unwrappedY[i]>maxBar) {
                            heightIncreased = true;
                            maxBar=unwrappedY[i];                                                        
                        }
                    }
                    var data = { y: unwrappedY, color: x, colorPalette: colorPalette, barWidth: 0.9, shadow: 'grey', shape: barMarker };
                    var plot2 = element.plot;
                    plot2.isAutoFitEnabled = false;                    
                    plot2.draw(data);
                    if (heightIncreased) {
                        plot2.maxBar = maxBar;
                        plot2.navigation.setVisibleRect({ x: -1, y: -maxBar*0.1, width: len + 1, height: maxBar * 1.2 });
                    }
                    
                }
            }
        }
    };

    ko.bindingHandlers.iddBarChartNames = {
        update: function (element, valueAccessor, allBindings, viewModel, bindingContext) {
            var value = valueAccessor();
            var unwrappedNames = ko.unwrap(value);

            var plotAttr = element.getAttribute("data-idd-plot");
            if (plotAttr != null) {
                if (typeof element.plot != 'undefined') {
                    var ticks = [];
                    var len = unwrappedNames.length;
                    for (var i = 0; i < len; i++)
                        ticks.push(i);
                    var plot = element.plot.master
                    var currentAxes = plot.getAxes("bottom");
                    if (typeof currentAxes !== 'undefined' && currentAxes.length>0)
                        currentAxes[0].remove();
                    if(len>0)
                        plot.addAxis("bottom", "labels", { labels: unwrappedNames, ticks: ticks });                    
                }
            }
        }
    };

    window.IntToColour = function (index) { //TODO: find other way to deliver the function to the KO komponent templates
        
        return colors[ko.unwrap(index)];
    };
});