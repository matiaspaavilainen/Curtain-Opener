package com.example.curtainopener

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.rounded.Home
import androidx.compose.material.icons.rounded.Refresh
import androidx.compose.material.icons.rounded.Settings
import androidx.compose.material3.BottomAppBar
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.curtainopener.ui.theme.CurtainOpenerTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            CurtainOpenerTheme {
                Surface(
                    modifier = Modifier
                        .fillMaxSize()
                        .background(MaterialTheme.colorScheme.background)
                ) {
                    HomePage()
                }
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun HomePage(viewModel: HttpClientViewModel = viewModel()) {
    val uiState by viewModel.uiState.collectAsStateWithLifecycle()
    // Get data from server immediately
    viewModel.getStatus()

    Scaffold(
        topBar = {
            CenterAlignedTopAppBar(
                colors = TopAppBarDefaults.centerAlignedTopAppBarColors(
                containerColor = MaterialTheme.colorScheme.primaryContainer,
                titleContentColor = MaterialTheme.colorScheme.primary,
            ), title = {
                Text(
                    "Curtain Opener", maxLines = 1, overflow = TextOverflow.Ellipsis
                )
            }, actions = {
                IconButton(onClick = { /* do something */ }) {
                    Icon(
                        imageVector = Icons.Rounded.Settings,
                        contentDescription = "Localized description"
                    )
                }
            })
        },

        bottomBar = {
            BottomAppBar(
                actions = {
                    IconButton(onClick = { viewModel.getStatus() }) {
                        Icon(Icons.Rounded.Refresh, contentDescription = "Localized description")
                    }
                    IconButton(onClick = { /* do something */ }) {
                        Icon(
                            Icons.Rounded.Home,
                            contentDescription = "Localized description",
                        )
                    }
                })
        },

        ) { innerPadding ->
        val basicTextBox = Modifier
            .border(
                width = 1.dp,
                color = MaterialTheme.colorScheme.primary,
                shape = RoundedCornerShape(size = 8.dp)
            )
            .clip(RoundedCornerShape(size = 8.dp))
            .background(MaterialTheme.colorScheme.primaryContainer)
            .padding(8.dp)

        Column(
            modifier = Modifier
                .padding(innerPadding)
                .padding(8.dp)
        ) {
            Weather(
                temperature = "${uiState.temperature} C", humidity = "${uiState.humidity} %"
            )

            Spacer(modifier = Modifier.height(8.dp))

            val weekdays =
                listOf("Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday")

            Column(verticalArrangement = Arrangement.spacedBy(8.dp)) {
                weekdays.zip(uiState.openingTimes).forEach { (day, time) ->
                    TimeRow(day, time, modifier = basicTextBox)
                }
            }


            Spacer(modifier = Modifier.height(8.dp))

            Row {
                Text(
                    text = "Alarm status: ${uiState.alarmStatus}",
                    modifier = basicTextBox.weight(1f)
                )

                Text(
                    text = "Time synced: ${uiState.timeCorrect}", modifier = basicTextBox.weight(1f)
                )
            }
        }
    }
}

@Composable
fun TimeRow(day: String, time: String, modifier: Modifier) {
    Text(text = "${day}: $time", modifier = modifier.fillMaxWidth())
}

@Preview
@Composable
fun BottomBarPrev() {
    HomePage()
}
  