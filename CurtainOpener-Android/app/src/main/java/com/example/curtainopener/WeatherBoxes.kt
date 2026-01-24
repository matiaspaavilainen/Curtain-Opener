package com.example.curtainopener

import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.wrapContentHeight
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp

@Composable
fun Weather(temperature: String, humidity: String) {
    val modifier = Modifier
        .border(
            width = 1.dp,
            color = MaterialTheme.colorScheme.primary,
            shape = RoundedCornerShape(size = 8.dp)
        )
        .clip(RoundedCornerShape(size = 8.dp))
        .background(MaterialTheme.colorScheme.primaryContainer)
        .padding(8.dp)
        .aspectRatio(1f)
        .wrapContentHeight()
    Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
        Column(
            verticalArrangement = Arrangement.Bottom,
            modifier = modifier
                .weight(1f)
        ) {
            Text(
                text = "Temperature",

            )
            Text(
                text = temperature,
                fontSize = 36.sp,
            )
        }
        Column(
            modifier = modifier
                .weight(1f)
        ) {
            Text(
                text = "Humidity"
            )
            Text(
                text = humidity,
                fontSize = 36.sp,
            )
        }
    }
}

@Preview
@Composable
fun WeatherPrev() {
    Weather(
        temperature = "21.3 °C", humidity = "45.7 %"
    )
}
